


APP=delaboratory

all: ${APP}

install:
	cp ${APP} /usr/local/bin

clean:
	rm -f ./.depend
	touch .depend
	rm -f *.o ${APP}

FILES = delaboratory value size project main_frame image_io channel channel_manager layer source_image_layer layer_stack view_manager layer_grid_panel image_area_panel image_panel renderer color_space image scale_channel curves_layer curve curve_point curve_shape curve_function_bezier action_frame curves_editor curves_panel blend_mode blend_frame frame slider power rgb2xyz2lab apply_luminance_color conversion_layer convert_image copy_image copy_channel conversion_functions hsl_hsv histogram histogram_panel str layer_factory action_layer mixer mixer_layer mixer_editor mixer_editor_channel control_panel gradient_panel apply_image_layer apply_image_frame blur blur_type process_linear cmyk blur_layer blur_frame usm_layer usm_frame view_mode_panel bw help_frame help_color_spaces_frame help_color_spaces_frame2 convert_pixel memory_info_frame sampler_manager sampler layer_frame help_color_spaces_frame3 help_color_spaces_frame4 help_color_spaces_frame5 skin_color_chart help_color_spaces_frame6 fractal xml conversion_bw_layer mixer_bw_editor histogram_mode_panel file_dialogs benchmark_frame benchmarks layer_processor property_value external_editor frame_factory property_value_slider property property_choice choice property_choice_ui property_boolean check_box property_boolean_ui dodge_burn_layer dodge_burn_frame blend_channel shadows_highlights_layer shadows_highlights_frame high_pass_layer high_pass_frame vignette_layer vignette samplers_panel sampler_panel layer_frame_manager logger vignette_frame conversion_bw2hue_layer hue_saturation_frame

ifeq (${OS}, WINDOWS)
# settings for Windows build

LDFLAGS=`/opt/wxw/bin/wx-config --libs` `pkg-config --libs libxml-2.0` -L/opt/lib -ltiff
CXXFLAGS=`/opt/wxw/bin/wx-config --cxxflags` `pkg-config --cflags libxml-2.0` -I/opt/include
OBJECTS = delab.o ${addsuffix .o, ${FILES}}

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

CXX=g++
WXCONFIG=wx-config
#WXCONFIG=wx-config-2.9


# choose your architecture
OPTFLAGS=-march=i686
#OPTFLAGS=-march=core2

# debug stuff
#CXXFLAGS+=-g -Wall -pedantic 

# release stuff
CXXFLAGS+=-Ofast ${OPTFLAGS} -DNDEBUG

# warnings from wxWidgets
CXXFLAGS+=-Wno-long-long -Wno-variadic-macros 

${APP}: ${OBJECTS}
	${CXX} -o ${APP} ${OBJECTS} ${LDFLAGS}

%.o: src/%.cc
	${CXX} -c ${CXXFLAGS} $<

depend: $(SOURCES) ${HEADERS}
		$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

include .depend
