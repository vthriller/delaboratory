
APP=delaboratory

all: ${APP} 

CXX=g++
WXCONFIG=wx-config
#WXCONFIG=wx-config-2.9

FILES = delaboratory main_frame image_io layer_grid_panel image_area_panel image_panel curves_panel frame slider histogram_panel control_panel gradient_panel view_mode_panel help_frame help_color_spaces_frame help_color_spaces_frame2 layer_frame help_color_spaces_frame3 help_color_spaces_frame4 help_color_spaces_frame5 help_color_spaces_frame6 histogram_mode_panel file_dialogs external_editor choice property_choice_ui check_box property_boolean_ui samplers_panel sampler_panel layer_frame_manager rendered_image dcraw_support raw_module zoom_panel threads_panel color_matrix_frame mutex color_space_utils semaphore main_window canvas canvas_wx bitmap bitmap_wx window window_wx panel_wx property_numeric_ui preset_button button warning_panel generic_layer_frame films property_levels_ui mutex_read_write property_mixer_ui palette_frame channel_selector property_curves_ui tiff logger str_wx update_main_layer_image layer_processor_threads progress_dialog message_box project update_blend

LAYERS = curves_layer mixer_layer auto_dodge_layer auto_burn_layer recover_shadows_layer recover_highlights_layer high_pass_layer vignette_layer source_image_layer gaussian_blur_layer gaussian_blur_single_layer conversion_layer equalizer_layer c2g_layer copy_layer gradient_layer fill_layer levels_layer tone_layer exposure_layer white_balance_layer saturation_layer local_contrast_layer sharpen_layer

ALGORITHMS = blur usm c2g scale_channel sample_pixel blend_channel vignette gradient blur_type blend_mode blend_color_luminosity fill_channel radial_lut

CORE = base_layer base_layer_with_properties base_layer_with_source channel_manager layer_stack conversion_cpu conversion_processor property color_space value image power size copy_channel histogram layer_factory operation_processor sampler_manager sampler preset static_image view_manager zoom_manager str switchable_layer color_matrix palette layer_processor flatten_layers layer_with_blending test_image skin_color_chart renderer

PROPERTIES = property_numeric property_mixer mixer property_curves curve curve_point curve_shape curve_function_bezier property_choice property_boolean property_levels

OBJECTS_TMP = ${addsuffix .o, ${FILES}}
OBJECTS = ${addprefix obj/, ${OBJECTS_TMP}}
LAYERS_OBJECTS_TMP = ${addsuffix .o, ${LAYERS}}
LAYERS_OBJECTS = ${addprefix obj/, ${LAYERS_OBJECTS_TMP}}
CORE_OBJECTS_TMP = ${addsuffix .o, ${CORE}}
CORE_OBJECTS = ${addprefix obj/, ${CORE_OBJECTS_TMP}}
PROPERTIES_OBJECTS_TMP = ${addsuffix .o, ${PROPERTIES}}
PROPERTIES_OBJECTS = ${addprefix obj/, ${PROPERTIES_OBJECTS_TMP}}
ALGORITHMS_OBJECTS_TMP = ${addsuffix .o, ${ALGORITHMS}}
ALGORITHMS_OBJECTS = ${addprefix obj/, ${ALGORITHMS_OBJECTS_TMP}}

CXXFLAGS=

ifeq (${OS}, WINDOWS)
# settings for Windows build

LDFLAGS=`/opt/wxw/bin/wx-config --libs` `pkg-config --libs libxml-2.0` -L/opt/lib -ltiff
CXXFLAGS_WX=`/opt/wxw/bin/wx-config --cxxflags` `pkg-config --cflags libxml-2.0` -I/opt/include
OBJECTS += delab.o 
EXE=.exe
else
# settings for standard build

LDFLAGS=`${WXCONFIG} --libs` `xml2-config --libs` -ltiff
CXXFLAGS_WX=`${WXCONFIG} --cxxflags` `xml2-config --cflags`

endif

# warnings from wxWidgets
CXXFLAGS_WX+=-Wno-long-long -Wno-variadic-macros 

INCLUDES=-I gui_wx -I layers -I algorithms -I core -I properties

CXXFLAGS += ${INCLUDES}
CXXFLAGS_WX += ${INCLUDES}

SOURCES_TMP = ${addsuffix .cc, ${FILES}}
SOURCES = ${addprefix gui_wx/, ${SOURCES_TMP}}
LAYERS_SOURCES_TMP = ${addsuffix .cc, ${LAYERS}}
LAYERS_SOURCES = ${addprefix layers/, ${LAYERS_SOURCES_TMP}}
CORE_SOURCES_TMP = ${addsuffix .cc, ${CORE}}
CORE_SOURCES = ${addprefix core/, ${CORE_SOURCES_TMP}}
PROPERTIES_SOURCES_TMP = ${addsuffix .cc, ${PROPERTIES}}
PROPERTIES_SOURCES = ${addprefix properties/, ${PROPERTIES_SOURCES_TMP}}
ALGORITHMS_SOURCES_TMP = ${addsuffix .cc, ${ALGORITHMS}}
ALGORITHMS_SOURCES = ${addprefix algorithms/, ${ALGORITHMS_SOURCES_TMP}}

HEADERS_TMP = ${addsuffix .h, ${FILES}}
HEADERS = ${addprefix gui_wx/, ${HEADERS_TMP}}
LAYERS_HEADERS_TMP = ${addsuffix .h, ${LAYERS}}
LAYERS_HEADERS = ${addprefix layers/, ${LAYERS_HEADERS_TMP}}
CORE_HEADERS_TMP = ${addsuffix .h, ${CORE}}
CORE_HEADERS = ${addprefix core/, ${CORE_HEADERS_TMP}}
PROPERTIES_HEADERS_TMP = ${addsuffix .h, ${PROPERTIES}}
PROPERTIES_HEADERS = ${addprefix properties/, ${PROPERTIES_HEADERS_TMP}}
ALGORITHMS_HEADERS_TMP = ${addsuffix .h, ${ALGORITHMS}}
ALGORITHMS_HEADERS = ${addprefix algorithms/, ${ALGORITHMS_HEADERS_TMP}}

STRIP=strip 


ifeq (${ARCH}, 64)
OPTFLAGS=-march=x86-64
else
OPTFLAGS=-march=i686
endif

# choose your architecture
#OPTFLAGS=-march=core2

ifeq (${DEBUG}, YES)
# debug stuff
CXXFLAGS+=-g -Wall -pedantic 
else
# release stuff
CXXFLAGS+=-Ofast ${OPTFLAGS} -DNDEBUG
endif


${APP}: ${CORE_OBJECTS} ${LAYERS_OBJECTS} ${ALGORITHMS_OBJECTS} ${PROPERTIES_OBJECTS} ${OBJECTS}
	${CXX} -o ${APP} ${CORE_OBJECTS} ${OBJECTS} ${LAYERS_OBJECTS} ${ALGORITHMS_OBJECTS} ${PROPERTIES_OBJECTS} ${LDFLAGS} 
ifeq (${DEBUG}, YES)
else
	${STRIP} ${APP}${EXE}
endif    

obj/%.o: gui_wx/%.cc
	${CXX} -c ${CXXFLAGS_WX} -o $@ $<

obj/%.o: layers/%.cc
	${CXX} -c ${CXXFLAGS} -o $@ $<

obj/%.o: core/%.cc
	${CXX} -c ${CXXFLAGS} -o $@ $<

obj/%.o: properties/%.cc
	${CXX} -c ${CXXFLAGS} -o $@ $<

obj/%.o: algorithms/%.cc
	${CXX} -c ${CXXFLAGS} -o $@ $<

depend: $(SOURCES) ${HEADERS}
		$(CXX) $(CXXFLAGS_WX) ${INCLUDES}  -MM $^>>./.depend;

include .depend

install:
	cp ${APP} /usr/local/bin

clean:
	rm -f ./.depend
	touch .depend
	rm -f ${APP}${EXE} 
	rm -f obj/*.o
        
    
