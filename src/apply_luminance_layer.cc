/*
    delaboratory - color correction utility
    Copyright (C) 2011 Jacek Poplawski

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "apply_luminance_layer.h"
#include "apply_luminance_color.h"
#include "copy_channel.h"
#include "property_value.h"
#include "color_space_utils.h"
#include "channel_manager.h"
#include "preset.h"

deApplyLuminanceLayer::deApplyLuminanceLayer(deColorSpace _colorSpace, int _sourceLayer, deLayerStack& _layerStack, deChannelManager& _channelManager)
:deLayerWithBlending(_colorSpace, _channelManager, _sourceLayer, _layerStack)
{
    dePreset* reset = createPreset("reset");
    int n = getColorSpaceSize(colorSpace);

    int i;
    for (i = 0; i < n; i++)
    {
        std::string n = "mixer " + getChannelName(colorSpace, i);
        createPropertyNumeric(n, 0, 1);
        if (i == 1) // green / magenta
        {
            reset->addNumericValue(n, 1.0);
        }
        else
        {
            reset->addNumericValue(n, 0.0);
        }
    }

    applyPreset("reset");
}

deApplyLuminanceLayer::~deApplyLuminanceLayer()
{
}

bool deApplyLuminanceLayer::processActionFull(deValue** sp, deValue** dp, int channelSize)
{
    if (colorSpace == deColorSpaceRGB)
    {
        int i;
        deValue mr = getNumericValue("mixer red");
        deValue mg = getNumericValue("mixer green");
        deValue mb = getNumericValue("mixer blue");
        for (i = 0; i < channelSize; i++)
        {
            deValue r = sp[0][i];
            deValue g = sp[1][i];
            deValue b = sp[2][i];
            deValue l = mr * r + mg * g + mb * b;
            applyLuminanceRGB(sp[0][i], sp[1][i], sp[2][i], l, l, l, dp[0][i], dp[1][i], dp[2][i]);
        }
        return true;
    }

    if (colorSpace == deColorSpaceProPhoto)
    {
        int i;
        deValue mr = getNumericValue("mixer red");
        deValue mg = getNumericValue("mixer green");
        deValue mb = getNumericValue("mixer blue");
        for (i = 0; i < channelSize; i++)
        {
            deValue r = sp[0][i];
            deValue g = sp[1][i];
            deValue b = sp[2][i];
            deValue l = mr * r + mg * g + mb * b;
            applyLuminanceProPhoto(sp[0][i], sp[1][i], sp[2][i], l, l, l, dp[0][i], dp[1][i], dp[2][i]);
        }
        return true;
    }

    if (colorSpace == deColorSpaceCMYK)
    {
    /*
        int i;
        deValue mc = mixer[0]->get();
        deValue mm = mixer[1]->get();
        deValue my = mixer[2]->get();
        deValue mk = mixer[3]->get();
        for (i = 0; i < channelSize; i++)
        {
            deValue c = sp[0][i];
            deValue m = sp[1][i];
            deValue y = sp[2][i];
            deValue k = sp[3][i];
            deValue l = mc * c + mm * m + my * y + mk * k;
            applyLuminanceCMYK(sp[0][i], sp[1][i], sp[2][i], sp[3][i], l, l, l, k, dp[0][i], dp[1][i], dp[2][i], dp[3][i]);
        }
        return true;
        */
    }

    int n = getColorSpaceSize(colorSpace);
    int i;
    for (i = 0; i < n; i++)
    {
        copyChannel(sp[i], dp[i], channelSize);
    }

    return true;
}

void deApplyLuminanceLayer::save(xmlNodePtr root)
{
    saveCommon(root);
    saveBlend(root);
}

void deApplyLuminanceLayer::load(xmlNodePtr root)
{
    loadBlend(root);

    xmlNodePtr child = root->xmlChildrenNode;

    while (child)
    {

        child = child->next;
    }
}


bool deApplyLuminanceLayer::updateMainImageNotThreadedWay()
{
    if (!isEnabled())
    {
        return true;
    }

    int channelSize = mainLayerImage.getChannelSize().getN();

    int s[MAX_COLOR_SPACE_SIZE];
    deChannel* sc[MAX_COLOR_SPACE_SIZE];
    int d[MAX_COLOR_SPACE_SIZE];
    deChannel* dc[MAX_COLOR_SPACE_SIZE];

    deValue* sp[MAX_COLOR_SPACE_SIZE];
    deValue* dp[MAX_COLOR_SPACE_SIZE];

    int n = getColorSpaceSize(colorSpace);
    int i;

    for (i = 0; i < n; i++)
    {
        s[i] = getSourceImage().getChannelIndex(i);
        sc[i] = channelManager.getChannel(s[i]);
        mainLayerImage.enableChannel(i);
        d[i] = mainLayerImage.getChannelIndex(i);
        dc[i] = channelManager.getChannel(d[i]);

        if (sc[i])
        {
            sp[i] = sc[i]->getPixels();
        }                    
        else
        {
            sp[i] = NULL;
        }

        if (dc[i])
        {
            dp[i] = dc[i]->getPixels();
        }                 
        else
        {
            dp[i] = NULL;
        }
    }

    bool actionResult = processActionFull(sp, dp, channelSize);

    return actionResult;
}
