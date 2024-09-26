/*
   Copyright (C) 1999-2007 id Software, Inc. and contributors.
   For a list of contributors, see the accompanying CONTRIBUTORS file.

   This file is part of GtkRadiant.

   GtkRadiant is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   GtkRadiant is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with GtkRadiant; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

// =============================================================================
// global tables

#include "plugin.h"

_QERFuncTable_1 g_FuncTable;
_QERFileSystemTable g_FileSystemTable;

// =============================================================================
// SYNAPSE

#include "synapse.h"

class CSynapseClientImage : public CSynapseClient
{
public:
// CSynapseClient API
bool RequestAPI( APIDescriptor_t *pAPI );
const char* GetInfo();

CSynapseClientImage() { }
virtual ~CSynapseClientImage() { }
};

CSynapseServer* g_pSynapseServer = NULL;
CSynapseClientImage g_SynapseClient;

#if __GNUC__ >= 4
#pragma GCC visibility push(default)
#endif
extern "C" CSynapseClient * SYNAPSE_DLL_EXPORT Synapse_EnumerateInterfaces( const char *version, CSynapseServer *pServer ) {
#if __GNUC__ >= 4
#pragma GCC visibility pop
#endif
	if ( strcmp( version, SYNAPSE_VERSION ) ) {
		Syn_Printf( "ERROR: synapse API version mismatch: should be '" SYNAPSE_VERSION "', got '%s'\n", version );
		return NULL;
	}
	g_pSynapseServer = pServer;
	g_pSynapseServer->IncRef();
	Set_Syn_Printf( g_pSynapseServer->Get_Syn_Printf() );

	g_SynapseClient.AddAPI( IMAGE_MAJOR, "jxl", sizeof( _QERPlugImageTable ) );
	g_SynapseClient.AddAPI( RADIANT_MAJOR, NULL, sizeof( _QERFuncTable_1 ), SYN_REQUIRE, &g_FuncTable );
	g_SynapseClient.AddAPI( VFS_MAJOR, "*", sizeof( _QERFileSystemTable ), SYN_REQUIRE, &g_FileSystemTable );

	return &g_SynapseClient;
}

bool CSynapseClientImage::RequestAPI( APIDescriptor_t *pAPI ){
	if ( !strcmp( pAPI->major_name, IMAGE_MAJOR ) ) {
		_QERPlugImageTable* pTable = static_cast<_QERPlugImageTable*>( pAPI->mpTable );
		if ( !strcmp( pAPI->minor_name, "jxl" ) ) {
			pTable->m_pfnLoadImage = &LoadImage;
			return true;
		}
	}

	Syn_Printf( "ERROR: RequestAPI( '%s' ) not found in '%s'\n", pAPI->major_name, GetInfo() );
	return false;
}

#include "version.h"

const char* CSynapseClientImage::GetInfo(){
	return "JXL loader module built " __DATE__ " " RADIANT_VERSION;
}

// ====== JXL loader functionality ======

#include <jxl/decode.h>

#define JXLD(exec, test) m_stat = exec; if (m_stat != test) { g_FuncTable.m_pfnSysPrintf("JXLReader: unexpected status code (%i) returned from " #exec " while trying to load \"%s\"", m_stat, filename); return; };
#define JXLDE(exec) m_stat = exec; if (m_stat != JXL_DEC_SUCCESS) { g_FuncTable.m_pfnSysPrintf("JXLReader: unexpected status code (%i) returned from " #exec " while trying to load \"%s\"", m_stat, filename); return;; };

struct JXLReader {
	
	void read(char const * filename, unsigned char * * data, int * width, int * height) {
		
		int nLen = g_FileSystemTable.m_pfnLoadFile( (char *)filename, (void **)&m_input, 0 );
		if ( nLen == -1 ) {
			return;
		}
		
		auto sig = JxlSignatureCheck(m_input, nLen);
		if (sig != JXL_SIG_CODESTREAM && sig != JXL_SIG_CONTAINER) return;
		
		size_t buffer_size = 0;
		
		m_dec = JxlDecoderCreate(nullptr);
		//assert(m_dec);
		
		JXLDE(JxlDecoderSetInput(m_dec, m_input, nLen));
		JXLDE(JxlDecoderSubscribeEvents(m_dec, JXL_DEC_BASIC_INFO | JXL_DEC_FULL_IMAGE));
		JXLD(JxlDecoderProcessInput(m_dec), JXL_DEC_BASIC_INFO);
		JXLDE(JxlDecoderGetBasicInfo(m_dec, &m_info));
		
		*width = m_info.xsize;
		*height = m_info.ysize;
		
		JXLD(JxlDecoderProcessInput(m_dec), JXL_DEC_NEED_IMAGE_OUT_BUFFER);
		
		static constexpr JxlPixelFormat PFMT {
			.num_channels = 4,
			.data_type = JXL_TYPE_UINT8,
			.endianness = JXL_NATIVE_ENDIAN,
			.align = 0
		};
		
		JXLDE(JxlDecoderImageOutBufferSize(m_dec, &PFMT, &buffer_size));
		
		*data = reinterpret_cast<uint8_t *>(g_malloc(buffer_size));
		
		JXLDE(JxlDecoderSetImageOutBuffer(m_dec, &PFMT, *data, buffer_size));
		JXLD(JxlDecoderProcessInput(m_dec), JXL_DEC_FULL_IMAGE);
		
		return;
	}
	
	~JXLReader() {
		if (m_dec) JxlDecoderDestroy(m_dec);
		if (m_input)
			g_FileSystemTable.m_pfnFreeFile( m_input );
	}
	
private:
	JxlDecoder * m_dec = nullptr;
	JxlDecoderStatus m_stat;
	JxlBasicInfo m_info;
	
	unsigned char * m_input = nullptr;
};

void LoadImage( const char *filename, unsigned char **pic, int *width, int *height ){
	JXLReader r;
	r.read(filename, pic, width, height);
}
