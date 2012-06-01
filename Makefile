
APP=delaboratory

all: ${APP} 

CXX=g++
WXCONFIG=wx-config
#WXCONFIG=wx-config-2.9

FILES = delaboratory value size project main_frame image_io channel channel_manager source_image_layer layer_stack view_manager layer_grid_panel image_area_panel image_panel renderer color_space image scale_channel curves_layer curve curve_point curve_shape curve_function_bezier curves_panel blend_mode frame slider power conversion_layer copy_image copy_channel histogram histogram_panel str layer_factory mixer mixer_layer control_panel gradient_panel blur blur_type process_linear view_mode_panel bw help_frame help_color_spaces_frame help_color_spaces_frame2 sampler_manager sampler layer_frame help_color_spaces_frame3 help_color_spaces_frame4 help_color_spaces_frame5 skin_color_chart help_color_spaces_frame6 fractal xml histogram_mode_panel file_dialogs benchmark_frame benchmarks layer_processor external_editor property property_choice choice property_choice_ui property_boolean check_box property_boolean_ui blend_channel high_pass_layer vignette_layer vignette samplers_panel sampler_panel layer_frame_manager static_image rendered_image dcraw_support raw_module zoom_panel zoom_manager equalizer_layer threads_panel palette color_matrix_frame color_matrix mutex color_space_utils base_layer semaphore main_window canvas canvas_wx operation_processor bitmap bitmap_wx switchable_layer base_layer_with_source layer_with_blending conversion_processor window window_wx panel_wx property_numeric property_numeric_ui preset preset_button button warning_panel generic_layer_frame levels_layer films property_levels property_levels_ui local_contrast_layer sharpen_layer usm fill_layer fill_channel gaussian_blur_layer gaussian_blur_single_layer conversion_cpu mutex_read_write tone_layer apply_original_layer test_image property_mixer property_mixer_ui palette_frame channel_selector property_curves property_curves_ui gradient gradient_layer exposure_layer white_balance_layer saturation_layer auto_dodge_layer auto_burn_layer recover_shadows_layer recover_highlights_layer blend_color_luminosity c2g_layer c2g radial_lut base_layer_with_properties

ifeq (${OS}, WINDOWS)
# settings for Windows build

LDFLAGS=`/opt/wxw/bin/wx-config --libs` `pkg-config --libs libxml-2.0` -L/opt/lib -ltiff
CXXFLAGS=`/opt/wxw/bin/wx-config --cxxflags` `pkg-config --cflags libxml-2.0` -I/opt/include
OBJECTS = delab.o ${addsuffix .o, ${FILES}}
EXE=.exe
else
# settings for standard build

LDFLAGS=`${WXCONFIG} --libs` `xml2-config --libs` -ltiff
CXXFLAGS=`${WXCONFIG} --cxxflags` `xml2-config --cflags`
OBJECTS = ${addsuffix .o, ${FILES}}

endif

SOURCES_TMP = ${addsuffix .cc, ${FILES}}
SOURCES = ${addprefix src/, ${SOURCES_TMP}}

HEADERS_TMP = ${addsuffix .h, ${FILES}}
HEADERS = ${addprefix src/, ${HEADERS_TMP}}

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

# warnings from wxWidgets
CXXFLAGS+=-Wno-long-long -Wno-variadic-macros 

${APP}: ${OBJECTS}
	${CXX} -c ${CXXFLAGS} -o logger.o src/logger.cc -DLOGGING
#	${CXX} -o ${APP}-with-logs ${OBJECTS} ${LDFLAGS} logger.o
#	${CXX} -c ${CXXFLAGS} -o logger.o src/logger.cc 
	${CXX} -o ${APP} ${OBJECTS} ${LDFLAGS} logger.o
ifeq (${DEBUG}, YES)
else
#	${STRIP} ${APP}-with-logs${EXE}
	${STRIP} ${APP}${EXE}
endif    

test_wx:
	${CXX} test_wx.cc ${CXXFLAGS} ${LDFLAGS} -o test_wx

test_no_wx:
	${CXX} test_no_wx.cc -o test_no_wx

%.o: src/%.cc
	${CXX} -c ${CXXFLAGS} $<

depend: $(SOURCES) ${HEADERS}
		$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

include .depend

install:
	cp ${APP} /usr/local/bin

clean:
	rm -f ./.depend
	touch .depend
	rm -f *.o ${APP}${EXE} 
#	rm -f *.o ${APP}${EXE} ${APP}-with-logs${EXE}
