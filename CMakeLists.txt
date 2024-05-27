cmake_minimum_required(VERSION 3.15)
project(dogiradiant)

set( CMAKE_CONFIGURATION_TYPES "Debug;Release;Native" )

find_package(PkgConfig REQUIRED)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(GTK2 REQUIRED gtk+-2.0)
pkg_check_modules(XML2 REQUIRED libxml-2.0)
pkg_check_modules(GTKGL REQUIRED gtkglext-1.0)
pkg_check_modules(ATK REQUIRED atk)
pkg_check_modules(JPEG REQUIRED libjpeg)
pkg_check_modules(MINIZ REQUIRED minizip)
pkg_check_modules(PNG REQUIRED libpng)

set( PLATFORM_LIBS "" )
if( UNIX )
	set( PLATFORM_LIBS "-lX11" )
endif()

ENABLE_LANGUAGE( C )
ENABLE_LANGUAGE( CXX )

set( CMAKE_C_FLAGS   "-fPIC -std=gnu11 -Werror=implicit-function-declaration" )
set( CMAKE_CXX_FLAGS "-fPIC -std=c++14 -Wno-parentheses" )

set( CMAKE_C_FLAGS_DEBUG   "-Wall -Og -march=core2 -mtune=native -ggdb3" )
set( CMAKE_C_FLAGS_RELEASE "-w -O2 -march=core2 -mtune=generic" )
set( CMAKE_C_FLAGS_NATIVE  "-w -Ofast -march=native -mtune=native" )

set( CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_C_FLAGS_DEBUG}" )
set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE}" )
set( CMAKE_CXX_FLAGS_NATIVE  "${CMAKE_C_FLAGS_NATIVE}" )

if( NOT CMAKE_BUILD_TYPE )
	set( CMAKE_BUILD_TYPE Native )
endif()

message( "Using build configuration: ${CMAKE_BUILD_TYPE}" )

set( CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/install )

# ================================================================
# ----------------------------------------------------------------

# STATIC LIBRARIES

# ----------------------------------------------------------------
# ================================================================

# ================================
# CMDLIB
# ================================

set( cmdlibFiles
	"libs/cmdlib/cmdlib.cpp"
)

add_library( cmdlib STATIC ${cmdlibFiles} )

target_include_directories( cmdlib PUBLIC 
	"include"
	"libs"
)

# ================================
# DDSLIB
# ================================

set( ddslibFiles
	"libs/ddslib/ddslib.c"
)

add_library( ddslib STATIC ${ddslibFiles} )

target_include_directories( ddslib PUBLIC 
	"include"
	"libs"
)

# ================================
# L_NET
# ================================

set( lnetFiles
	"libs/l_net/l_net.c"
	"libs/l_net/l_net_berkeley.c"
)

add_library( lnet STATIC ${lnetFiles} )

target_include_directories( lnet PUBLIC 
	"include"
	"libs"
)

# ================================
# MATHLIB
# ================================

set( mathlibFiles
	"libs/mathlib/bbox.c"
	"libs/mathlib/m4x4.c"
	"libs/mathlib/mathlib.c"
	"libs/mathlib/ray.c"
)

add_library( mathlib STATIC ${mathlibFiles} )

target_include_directories( mathlib PUBLIC 
	"include"
	"libs"
)

target_link_libraries( mathlib PUBLIC
	"m"
)

# ================================
# MD5LIB
# ================================

set( md5libFiles
	"libs/md5lib/md4.c"
	"libs/md5lib/md5lib.c"
)

add_library( md5lib STATIC ${md5libFiles} )

target_include_directories( md5lib PUBLIC 
	"include"
	"libs"
)

# ================================
# PICOMODEL
# ================================

set( picomodelFiles
	"libs/picomodel/picointernal.c"
	"libs/picomodel/picomodel.c"
	"libs/picomodel/picomodules.c"
	"libs/picomodel/pm_3ds.c"
	"libs/picomodel/pm_ase.c"
	"libs/picomodel/pm_fm.c"
	"libs/picomodel/pm_lwo.c"
	"libs/picomodel/pm_md2.c"
	"libs/picomodel/pm_md3.c"
	"libs/picomodel/pm_mdc.c"
	"libs/picomodel/pm_ms3d.c"
	"libs/picomodel/pm_obj.c"
	"libs/picomodel/lwo/clip.c"
	"libs/picomodel/lwo/envelope.c"
	"libs/picomodel/lwo/list.c"
	"libs/picomodel/lwo/lwio.c"
	"libs/picomodel/lwo/lwo2.c"
	"libs/picomodel/lwo/lwob.c"
	"libs/picomodel/lwo/pntspols.c"
	"libs/picomodel/lwo/surface.c"
	"libs/picomodel/lwo/vecmath.c"
	"libs/picomodel/lwo/vmap.c"
	"libs/picomodel/pm_terrain.c"
)

add_library( picomodel STATIC ${picomodelFiles} )

target_include_directories( picomodel PUBLIC 
	"include"
	"libs"
)

# ================================
# SPLINES
# ================================

set( splinesFiles
	"libs/splines/math_angles.cpp"
	"libs/splines/math_matrix.cpp"
	"libs/splines/math_quaternion.cpp"
	"libs/splines/math_vector.cpp"
	"libs/splines/q_parse.cpp"
	"libs/splines/q_shared.cpp"
	"libs/splines/splines.cpp"
	"libs/splines/util_str.cpp"
)

add_library( splines STATIC ${splinesFiles} )

target_include_directories( splines PUBLIC 
	"include"
	"libs"
)

# ================================
# SYNAPSE
# ================================

set( synapseFiles
	"libs/synapse/synapse.cpp"
)

add_library( synapse STATIC ${synapseFiles} )

target_include_directories( synapse PUBLIC
	${GLIB2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( synapse PUBLIC 
	${GLIB2_LIBRARIES}
	${XML2_LIBRARIES}
)

# ================================================================
# ----------------------------------------------------------------

# MODULES

# ----------------------------------------------------------------
# ================================================================

# ================================
# BKGRND2D
# ================================

set( bkgrnd2dFiles
	"contrib/bkgrnd2d/bkgrnd2d.cpp"
	"contrib/bkgrnd2d/dialog.cpp"
	"contrib/bkgrnd2d/plugin.cpp"
)

add_library( bkgrnd2d SHARED ${bkgrnd2dFiles} )
set_property( TARGET bkgrnd2d PROPERTY PREFIX "" )

target_include_directories( bkgrnd2d PUBLIC
	${ATK_INCLUDE_DIRS}
	${GLIB2_INCLUDE_DIRS}
	${GTK2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( bkgrnd2d PUBLIC
	${ATK_LIBRARIES}
	${GLIB2_LIBRARIES}
	${GTK2_LIBRARIES}
	${XML2_LIBRARIES}
	mathlib
	synapse
)

install( TARGETS bkgrnd2d LIBRARY DESTINATION modules )

# ================================
# BOBTOOLZ
# ================================

set( bobtoolzFiles
	"contrib/bobtoolz/bobToolz-GTK.cpp"
	"contrib/bobtoolz/bsploader.cpp"
	"contrib/bobtoolz/cportals.cpp"
	"contrib/bobtoolz/DBobView.cpp"
	"contrib/bobtoolz/DBrush.cpp"
	"contrib/bobtoolz/DEntity.cpp"
	"contrib/bobtoolz/DEPair.cpp"
	"contrib/bobtoolz/DListener.cpp"
	"contrib/bobtoolz/DMap.cpp"
	"contrib/bobtoolz/DPatch.cpp"
	"contrib/bobtoolz/DPlane.cpp"
	"contrib/bobtoolz/DPoint.cpp"
	"contrib/bobtoolz/DShape.cpp"
	"contrib/bobtoolz/DTrainDrawer.cpp"
	"contrib/bobtoolz/DTreePlanter.cpp"
	"contrib/bobtoolz/DVisDrawer.cpp"
	"contrib/bobtoolz/DWinding.cpp"
	"contrib/bobtoolz/funchandlers-GTK.cpp"
	"contrib/bobtoolz/lists.cpp"
	"contrib/bobtoolz/misc.cpp"
	"contrib/bobtoolz/ScriptParser.cpp"
	"contrib/bobtoolz/shapes.cpp"
	"contrib/bobtoolz/StdAfx.cpp"
	"contrib/bobtoolz/visfind.cpp"
	"contrib/bobtoolz/dialogs/dialogs-gtk.cpp"
)

add_library( bobtoolz SHARED ${bobtoolzFiles} )
set_property( TARGET bobtoolz PROPERTY PREFIX "" )

target_include_directories( bobtoolz PUBLIC
	${ATK_INCLUDE_DIRS}
	${GLIB2_INCLUDE_DIRS}
	${GTK2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( bobtoolz PUBLIC
	${ATK_LIBRARIES}
	${GLIB2_LIBRARIES}
	${GTK2_LIBRARIES}
	${XML2_LIBRARIES}
	cmdlib
	mathlib
	synapse
)

install( TARGETS bobtoolz LIBRARY DESTINATION modules )

# ================================
# CAMERA
# ================================

set( cameraFiles
	"contrib/camera/camera.cpp"
	"contrib/camera/dialogs.cpp"
	"contrib/camera/dialogs_common.cpp"
	"contrib/camera/funchandlers.cpp"
	"contrib/camera/listener.cpp"
	"contrib/camera/misc.cpp"
	"contrib/camera/renderer.cpp"
)

add_library( camera SHARED ${cameraFiles} )
set_property( TARGET camera PROPERTY PREFIX "" )

target_include_directories( camera PUBLIC
	${ATK_INCLUDE_DIRS}
	${GLIB2_INCLUDE_DIRS}
	${GTK2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( camera PUBLIC
	${ATK_LIBRARIES}
	${GLIB2_LIBRARIES}
	${GTK2_LIBRARIES}
	${XML2_LIBRARIES}
	splines
	synapse
)

install( TARGETS camera LIBRARY DESTINATION modules )

# ================================
# ENTITY
# ================================

set( entityFiles
	"plugins/entity/eclassmodel.cpp"
	"plugins/entity/entity.cpp"
	"plugins/entity/entity_entitymodel.cpp"
	"plugins/entity/light.cpp"
	"plugins/entity/miscmodel.cpp"
	"plugins/entity/plugin.cpp"
)

add_library( entity SHARED ${entityFiles} )
set_property( TARGET entity PROPERTY PREFIX "" )

target_include_directories( entity PUBLIC
	${GLIB2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( entity PUBLIC 
	${GLIB2_LIBRARIES}
	${XML2_LIBRARIES}
	mathlib
	synapse
)

install( TARGETS entity LIBRARY DESTINATION modules )

# ================================
# GTKGENSURF
# ================================

set( gtkgensurfFiles
	"contrib/gtkgensurf/bitmap.cpp"
	"contrib/gtkgensurf/dec.cpp"
	"contrib/gtkgensurf/face.cpp"
	"contrib/gtkgensurf/font.cpp"
	"contrib/gtkgensurf/gendlgs.cpp"
	"contrib/gtkgensurf/genmap.cpp"
	"contrib/gtkgensurf/gensurf.cpp"
	"contrib/gtkgensurf/heretic.cpp"
	"contrib/gtkgensurf/plugin.cpp"
	"contrib/gtkgensurf/triangle.c"
	"contrib/gtkgensurf/view.cpp"
)

add_library( gtkgensurf SHARED ${gtkgensurfFiles} )
set_property( TARGET gtkgensurf PROPERTY PREFIX "" )

target_include_directories( gtkgensurf PUBLIC
	${ATK_INCLUDE_DIRS}
	${GLIB2_INCLUDE_DIRS}
	${GTK2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( gtkgensurf PUBLIC
	${ATK_LIBRARIES}
	${GLIB2_LIBRARIES}
	${GTK2_LIBRARIES}
	${XML2_LIBRARIES}
	synapse
)

install( TARGETS gtkgensurf LIBRARY DESTINATION modules )

# ================================
# HYDRATOOLZ
# ================================

set( hydratoolzFiles
	"contrib/hydratoolz/plugin.cpp"
)

add_library( hydratoolz SHARED ${hydratoolzFiles} )
set_property( TARGET hydratoolz PROPERTY PREFIX "" )

target_include_directories( hydratoolz PUBLIC
	${ATK_INCLUDE_DIRS}
	${GLIB2_INCLUDE_DIRS}
	${GTK2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( hydratoolz PUBLIC
	${ATK_LIBRARIES}
	${GLIB2_LIBRARIES}
	${GTK2_LIBRARIES}
	${XML2_LIBRARIES}
	synapse
)

install( TARGETS hydratoolz LIBRARY DESTINATION modules )

# ================================
# IMAGE
# ================================

set( imageFiles
	"plugins/image/bmp.cpp"
	"plugins/image/image.cpp"
	"plugins/image/jpeg.cpp"
	"plugins/image/lbmlib.cpp"
)

add_library( image SHARED ${imageFiles} )
set_property( TARGET image PROPERTY PREFIX "" )

target_include_directories( image PUBLIC
	${GLIB2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	${JPEG_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( image PUBLIC 
	${GLIB2_LIBRARIES}
	${XML2_LIBRARIES}
	${JPEG_LIBRARIES}
	synapse
)

install( TARGETS image LIBRARY DESTINATION modules )

# ================================
# IMAGEPNG
# ================================

set( imagepngFiles
	"plugins/imagepng/plugin.cpp"
)

add_library( imagepng SHARED ${imagepngFiles} )
set_property( TARGET imagepng PROPERTY PREFIX "" )

target_include_directories( imagepng PUBLIC
	${GLIB2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	${PNG_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( imagepng PUBLIC 
	${GLIB2_LIBRARIES}
	${XML2_LIBRARIES}
	${PNG_LIBRARIES}
	synapse
)

install( TARGETS imagepng LIBRARY DESTINATION modules )

# ================================
# MAP
# ================================

set( mapFiles
	"plugins/map/parse.cpp"
	"plugins/map/plugin.cpp"
	"plugins/map/write.cpp"
)

add_library( map SHARED ${mapFiles} )
set_property( TARGET map PROPERTY PREFIX "" )

target_include_directories( map PUBLIC
	${GLIB2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( map PUBLIC 
	${GLIB2_LIBRARIES}
	${XML2_LIBRARIES}
	cmdlib
	synapse
)

install( TARGETS map LIBRARY DESTINATION modules )

# ================================
# MAPXML
# ================================

set( mapxmlFiles
	"plugins/mapxml/plugin.cpp"
	"plugins/mapxml/xmlparse.cpp"
	"plugins/mapxml/xmlwrite.cpp"
)

add_library( mapxml SHARED ${mapxmlFiles} )
set_property( TARGET mapxml PROPERTY PREFIX "" )

target_include_directories( mapxml PUBLIC
	${GLIB2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( mapxml PUBLIC 
	${GLIB2_LIBRARIES}
	${XML2_LIBRARIES}
	synapse
)

install( TARGETS mapxml LIBRARY DESTINATION modules )

# ================================
# MODEL
# ================================

set( modelFiles
	"plugins/model/cpicomodel.cpp"
	"plugins/model/cpicosurface.cpp"
	"plugins/model/model.cpp"
	"plugins/model/plugin.cpp"
	"plugins/model/remap.cpp"
)

add_library( model SHARED ${modelFiles} )
set_property( TARGET model PROPERTY PREFIX "" )

target_include_directories( model PUBLIC
	${GLIB2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( model PUBLIC 
	${GLIB2_LIBRARIES}
	${XML2_LIBRARIES}
	mathlib
	synapse
	picomodel
)

install( TARGETS model LIBRARY DESTINATION modules )

# ================================
# PRTVIEW
# ================================

set( prtviewFiles
	"contrib/prtview/AboutDialog.cpp"
	"contrib/prtview/ConfigDialog.cpp"
	"contrib/prtview/LoadPortalFileDialog.cpp"
	"contrib/prtview/portals.cpp"
	"contrib/prtview/prtview.cpp"
	"contrib/prtview/stdafx.cpp"
)

add_library( prtview SHARED ${prtviewFiles} )
set_property( TARGET prtview PROPERTY PREFIX "" )

target_include_directories( prtview PUBLIC
	${GLIB2_INCLUDE_DIRS}
	${GTK2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( prtview PUBLIC
	${GLIB2_LIBRARIES}
	${GTK2_LIBRARIES}
	${XML2_LIBRARIES}
	synapse
)

install( TARGETS prtview LIBRARY DESTINATION modules )

# ================================
# SHADERS
# ================================

set( shadersFiles
	"plugins/shaders/shaders.cpp"
	"plugins/shaders/plugin.cpp"
)

add_library( shaders SHARED ${shadersFiles} )
set_property( TARGET shaders PROPERTY PREFIX "" )

target_include_directories( shaders PUBLIC
	${GLIB2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( shaders PUBLIC 
	${GLIB2_LIBRARIES}
	${XML2_LIBRARIES}
	synapse
)

install( TARGETS shaders LIBRARY DESTINATION modules )

# ================================
# SPRITEMODEL
# ================================

set( spritemodelFiles
	"plugins/spritemodel/plugin.cpp"
	"plugins/spritemodel/spritemodel.cpp"
)

add_library( spritemodel SHARED ${spritemodelFiles} )
set_property( TARGET spritemodel PROPERTY PREFIX "" )

target_include_directories( spritemodel PUBLIC
	${GLIB2_INCLUDE_DIRS}
	${GTK2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( spritemodel PUBLIC 
	${GLIB2_LIBRARIES}
	${XML2_LIBRARIES}
	mathlib
	synapse
)

install( TARGETS spritemodel LIBRARY DESTINATION modules )

# ================================
# SURFACE
# ================================

set( surfaceFiles
	"plugins/surface/surfacedialog.cpp"
	"plugins/surface/surfdlg_plugin.cpp"
)

add_library( surface SHARED ${surfaceFiles} )
set_property( TARGET surface PROPERTY PREFIX "" )

target_include_directories( surface PUBLIC 
	${GLIB2_INCLUDE_DIRS}
	${GTK2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( surface PUBLIC 
	${GLIB2_LIBRARIES}
	${GTK2_LIBRARIES}
	${XML2_LIBRARIES}
	synapse
)

install( TARGETS surface LIBRARY DESTINATION modules )

# ================================
# TEXTOOL
# ================================

set( textoolFiles
	"plugins/textool/2DView.cpp"
	"plugins/textool/ControlPointsManager.cpp"
	"plugins/textool/StdAfx.cpp"
	"plugins/textool/TexTool.cpp"
)

add_library( textool SHARED ${textoolFiles} )
set_property( TARGET textool PROPERTY PREFIX "" )

target_include_directories( textool PUBLIC
	${GLIB2_INCLUDE_DIRS}
	${GTK2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( textool PUBLIC 
	${GLIB2_LIBRARIES}
	${GTK2_LIBRARIES}
	${XML2_LIBRARIES}
	mathlib
	synapse
)

install( TARGETS textool LIBRARY DESTINATION modules )

# ================================
# VFSPK3
# ================================

set( vfspk3Files
	"plugins/vfspk3/vfs.cpp"
	"plugins/vfspk3/vfspk3.cpp"
)

add_library( vfspk3 SHARED ${vfspk3Files} )
set_property( TARGET vfspk3 PROPERTY PREFIX "" )

target_include_directories( vfspk3 PUBLIC 
	${GLIB2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	${MINIZ_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( vfspk3 PUBLIC 
	${GLIB2_LIBRARIES}
	${XML2_LIBRARIES}
	${MINIZ_LIBRARIES}
	synapse
)

install( TARGETS vfspk3 LIBRARY DESTINATION modules )

# ================================================================
# ----------------------------------------------------------------

# MAIN

# ----------------------------------------------------------------
# ================================================================

# ================================
# Q3COMMON
# ================================

set( q3commonFiles
	"tools/quake3/common/cmdlib.c"
	"tools/quake3/common/imagelib.c"
	"tools/quake3/common/inout.c"
	"tools/quake3/common/jpeg.c"
	"tools/quake3/common/mutex.c"
	"tools/quake3/common/polylib.c"
	"tools/quake3/common/scriplib.c"
	"tools/quake3/common/threads.c"
	"tools/quake3/common/vfs.c"
)

add_library( q3common STATIC ${q3commonFiles} )

target_include_directories( q3common PUBLIC 
	${GLIB2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	${JPEG_INCLUDE_DIRS}
	${Z_INCLUDE_DIRS}
	${MINIZ_INCLUDE_DIRS}
	"include"
	"libs"
)

target_link_libraries( q3common
	${GLIB2_LIBRARIES}
	${XML2_LIBRARIES}
	${JPEG_LIBRARIES}
	${Z_LIBRARIES}
	${MINIZ_LIBRARIES}
	"pthread"
)

target_compile_definitions( q3common
	PRIVATE Q3COMMON
)

# ================================
# Q3DATA
# ================================

set( q3dataFiles
	"tools/quake3/q3data/3dslib.c"
	"tools/quake3/common/aselib.c"
	"tools/quake3/common/bspfile.c"
	"tools/quake3/common/cmdlib.c"
	"tools/quake3/q3data/compress.c"
	"tools/quake3/common/imagelib.c"
	"tools/quake3/q3data/images.c"
	"tools/quake3/common/inout.c"
	"tools/quake3/q3data/md3lib.c"
	"tools/quake3/q3data/models.c"
	"tools/quake3/q3data/p3dlib.c"
	"tools/quake3/q3data/polyset.c"
	"tools/quake3/q3data/q3data.c"
	"tools/quake3/common/scriplib.c"
	"tools/quake3/q3data/stripper.c"
	"tools/quake3/common/trilib.c"
	"tools/quake3/common/vfs.c"
	"tools/quake3/q3data/video.c"
)

add_executable( q3data ${q3dataFiles} )

target_include_directories( q3data PUBLIC
	${GLIB2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	${JPEG_INCLUDE_DIRS}
	${MINIZ_INCLUDE_DIRS}
	"tools/quake3/common"
	"include"
	"libs"
)

target_link_libraries( q3data
	${GLIB2_LIBRARIES}
	${XML2_LIBRARIES}
	${JPEG_LIBRARIES}
	${MINIZ_LIBRARIES}
	"pthread"
	"m"
	q3common
	lnet
	mathlib
)

install( TARGETS q3data RUNTIME DESTINATION . )

# ================================
# Q3MAP2
# ================================

set( q3map2Files
	"tools/quake3/q3map2/bspfile_abstract.c"
	"tools/quake3/q3map2/bspfile_ibsp.c"
	"tools/quake3/q3map2/bspfile_rbsp.c"
	"tools/quake3/q3map2/image.c"
	"tools/quake3/q3map2/main.c"
	"tools/quake3/q3map2/mesh.c"
	"tools/quake3/q3map2/minimap.c"
	"tools/quake3/q3map2/model.c"
	"tools/quake3/q3map2/path_init.c"
	"tools/quake3/q3map2/shaders.c"
	"tools/quake3/q3map2/surface_extra.c"
	"tools/quake3/q3map2/brush.c"
	"tools/quake3/q3map2/brush_primit.c"
	"tools/quake3/q3map2/bsp.c"
	"tools/quake3/q3map2/bsp_analyze.c"
	"tools/quake3/q3map2/bsp_scale.c"
	"tools/quake3/q3map2/bsp_info.c"
	"tools/quake3/q3map2/decals.c"
	"tools/quake3/q3map2/facebsp.c"
	"tools/quake3/q3map2/fixaas.c"
	"tools/quake3/q3map2/fog.c"
	"tools/quake3/q3map2/leakfile.c"
	"tools/quake3/q3map2/map.c"
	"tools/quake3/q3map2/patch.c"
	"tools/quake3/q3map2/portals.c"
	"tools/quake3/q3map2/prtfile.c"
	"tools/quake3/q3map2/surface.c"
	"tools/quake3/q3map2/surface_foliage.c"
	"tools/quake3/q3map2/surface_fur.c"
	"tools/quake3/q3map2/surface_meta.c"
	"tools/quake3/q3map2/tjunction.c"
	"tools/quake3/q3map2/tree.c"
	"tools/quake3/q3map2/writebsp.c"
	"tools/quake3/q3map2/light.c"
	"tools/quake3/q3map2/light_bounce.c"
	"tools/quake3/q3map2/light_trace.c"
	"tools/quake3/q3map2/light_ydnar.c"
	"tools/quake3/q3map2/lightmaps_ydnar.c"
	"tools/quake3/q3map2/exportents.c"
	"tools/quake3/q3map2/vis.c"
	"tools/quake3/q3map2/visflow.c"
	"tools/quake3/q3map2/convert_ase.c"
	"tools/quake3/q3map2/convert_bsp.c"
	"tools/quake3/q3map2/convert_map.c"
)

add_executable( q3map2 ${q3map2Files} )

target_include_directories( q3map2 PUBLIC
	${GLIB2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	${JPEG_INCLUDE_DIRS}
	${PNG_INCLUDE_DIRS}
	"tools/quake3/common"
	"include"
	"libs"
)

target_link_libraries( q3map2
	${GLIB2_LIBRARIES}
	${XML2_LIBRARIES}
	${JPEG_LIBRARIES}
	${PNG_LIBRARIES}
	"pthread"
	"m"
	q3common
	ddslib
	lnet
	mathlib
	md5lib
	picomodel
)

install( TARGETS q3map2 RUNTIME DESTINATION . )

# ================================
# RADIANT
# ================================

set( radiantFiles
	"radiant/bp_dlg.cpp"
	"radiant/brush.cpp"
	"radiant/brush_primit.cpp"
	"radiant/brushscript.cpp"
	"radiant/camwindow.cpp"
	"radiant/csg.cpp"
	"radiant/dialog.cpp"
	"radiant/dialoginfo.cpp"
	"radiant/drag.cpp"
	"radiant/eclass.cpp"
	"radiant/eclass_def.cpp"
	"radiant/error.cpp"
	"radiant/feedback.cpp"
	"radiant/file.cpp"
	"radiant/filters.cpp"
	"radiant/findtexturedialog.cpp"
	"radiant/glinterface.cpp"
	"radiant/glwidget.cpp"
	"radiant/glwindow.cpp"
	"radiant/groupdialog.cpp"
	"radiant/gtkdlgs.cpp"
	"radiant/gtkmisc.cpp"
	"radiant/main.cpp"
	"radiant/mainframe.cpp"
	"radiant/map.cpp"
	"radiant/missing.cpp"
	"radiant/parse.cpp"
	"radiant/patchdialog.cpp"
	"radiant/pluginentities.cpp"
	"radiant/pluginmanager.cpp"
	"radiant/pmesh.cpp"
	"radiant/points.cpp"
	"radiant/preferences.cpp"
	"radiant/profile.cpp"
	"radiant/qe3.cpp"
	"radiant/qgl.c"
	"radiant/qgl_ext.cpp"
	"radiant/select.cpp"
	"radiant/selectedface.cpp"
	"radiant/stdafx.cpp"
	"radiant/surfacedialog.cpp"
	"radiant/surfaceplugin.cpp"
	"radiant/targetname.cpp"
	"radiant/texmanip.cpp"
	"radiant/texwindow.cpp"
	"radiant/ui.cpp"
	"radiant/undo.cpp"
	"radiant/vertsel.cpp"
	"radiant/watchbsp.cpp"
	"radiant/winding.cpp"
	"radiant/xywindow.cpp"
)

add_executable( radiant ${radiantFiles} )

target_include_directories( radiant PUBLIC
	${GLIB2_INCLUDE_DIRS}
	${GTK2_INCLUDE_DIRS}
	${XML2_INCLUDE_DIRS}
	${GTKGL_INCLUDE_DIRS}
	"radiant"
	"include"
	"libs"
)

target_link_libraries( radiant
	${GLIB2_LIBRARIES}
	${GTK2_LIBRARIES}
	${XML2_LIBRARIES}
	${GTKGL_LIBRARIES}
	${PLATFORM_LIBS}
	"dl"
	cmdlib
	lnet
	mathlib
	synapse
)

install( TARGETS radiant RUNTIME DESTINATION . )
