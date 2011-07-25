APP=delaboratory

all: ${APP}

install:
	cp ${APP} /usr/local/bin

clean:
	rm -f ./.depend
	touch .depend
	rm -f *.o ${APP}

FILES = delaboratory main_frame layer_list_panel right_panel left_panel top_panel bottom_panel histogram_panel size value channel preview source_image color_space exception project layer source_image_layer layer_stack image_panel add_layer_dialog mixer_layer mixer save_buttons_panel final_image sampler sampler_list_panel sampler_panel sampler_list conversion_layer convert_color renderer converter histogram blend_layer blend slider mixer_slider preview_stack info_bar_panel base_channel empty_channel histogram_mode_panel channel_buttons blend_mode channel_choice blend_mode_choice color_space_choice layer_choice curve_point curve curves curves_layer curves_panel curve_shape action_frame gui view_mode_panel center_panel logger blur_layer blur nd nd_layer nd_type nd_type_choice channel_check_boxes curve_function curve_function_spline property property_mixer property_nd property_curves property_value property_name property_layer_index property_color_space property_blend_mode property_channels layer_frame curves_editor mixer_editor color_space_selector layer_selector blend_mode_selector property_value_slider channels_selector name_editor property_nd_type nd_type_selector mixer_editor_channel layer_factory property_integer property_integer_slider str property_blend_mask mask_selector grain_layer grain blur_type property_blur_type blur_type_selector blur_type_choice high_pass_layer

OBJECTS = ${addsuffix .o, ${FILES}}

SOURCES_TMP = ${addsuffix .cc, ${FILES}}
SOURCES = ${addprefix src/, ${SOURCES_TMP}}

HEADERS_TMP = ${addsuffix .h, ${FILES}}
HEADERS = ${addprefix src/, ${HEADERS_TMP}}

CXX=g++
WXCONFIG=wx-config
#WXCONFIG=wx-config-2.9

LDFLAGS=`${WXCONFIG} --libs` `xml2-config --libs` -ltiff
CXXFLAGS=`${WXCONFIG} --cxxflags` `xml2-config --cflags`

# profler
#CXXFLAGS+= -DDE_PROFILER

# logs
#CXXFLAGS+= -DDE_LOGGER

# debug stuff
#CXXFLAGS+=-g -Wall -pedantic 

# release stuff
#CXXFLAGS+=-O2 -march=i686
CXXFLAGS+=-O2 

# warnings from wxWidgets
CXXFLAGS+=-Wno-long-long -Wno-variadic-macros 

${APP}: ${OBJECTS}
	${CXX} -o ${APP} ${OBJECTS} ${LDFLAGS}

%.o: src/%.cc
	${CXX} -c ${CXXFLAGS} $<

#.depend: depend

depend: $(SOURCES) ${HEADERS}
		$(CXX) $(CXXFLAGS) -MM $^>>./.depend;

include .depend
