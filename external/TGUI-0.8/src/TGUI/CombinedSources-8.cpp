/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2020 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Global.hpp>
#include <TGUI/Clipboard.hpp>
#include <TGUI/DefaultFont.hpp>
#include <functional>
#include <sstream>
#include <locale>
#include <cctype> // isspace
#include <cmath> // abs

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    namespace
    {
        unsigned int globalTextSize = 13;
        unsigned int globalDoubleClickTime = 500;
        unsigned int globalEditBlinkRate = 500;
        std::string globalResourcePath = "";
        std::shared_ptr<sf::Font> globalFont = nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void setGlobalTextSize(unsigned int textSize)
    {
        globalTextSize = textSize;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int getGlobalTextSize()
    {
        return globalTextSize;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void setGlobalFont(const Font& font)
    {
        globalFont = font.getFont();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Font getGlobalFont()
    {
        if (!globalFont)
        {
            globalFont = std::make_shared<sf::Font>();
            globalFont->loadFromMemory(defaultFontBytes, sizeof(defaultFontBytes));
        }

        return globalFont;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::shared_ptr<sf::Font>& getInternalGlobalFont()
    {
        return globalFont;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void setDoubleClickTime(unsigned int milliseconds)
    {
        globalDoubleClickTime = milliseconds;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int getDoubleClickTime()
    {
        return globalDoubleClickTime;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void setEditCursorBlinkRate(unsigned int blinkRateMilliseconds)
    {
        globalEditBlinkRate = blinkRateMilliseconds;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int getEditCursorBlinkRate()
    {
        return globalEditBlinkRate;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void setResourcePath(const std::string& path)
    {
        globalResourcePath = path;

        if (!globalResourcePath.empty())
        {
            if (globalResourcePath[globalResourcePath.length()-1] != '/')
                globalResourcePath.push_back('/');
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const std::string& getResourcePath()
    {
        return globalResourcePath;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool compareFloats(float x, float y)
    {
        return (std::abs(x - y) < 0.0000001f);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef TGUI_REMOVE_DEPRECATED_CODE
    int stoi(const std::string& value)
    {
        int result = 0;

        std::istringstream iss(value);
        iss.imbue(std::locale::classic());
        iss >> result;

        if (iss.fail())
            result = 0;

        return result;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float stof(const std::string& value)
    {
        float result = 0;

        std::istringstream iss(value);
        iss.imbue(std::locale::classic());
        iss >> result;

        if (iss.fail())
            result = 0;

        return result;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool extractBoolFromString(const std::string& property, const std::string& value)
    {
        if ((value == "true") || (value == "True") || (value == "TRUE") || (value == "1"))
            return true;
        else if ((value == "false") || (value == "False") || (value == "FALSE") || (value == "0"))
            return false;
        else
            throw Exception{"Failed to parse boolean value of property '" + property + "'."};
    }
#endif
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef TGUI_NEXT
    bool removeWhitespace(const std::string& line, std::string::const_iterator& c)
    {
        while (c != line.end())
        {
            if ((*c == ' ') || (*c == '\t') || (*c == '\r'))
                ++c;
            else
                return true;
        }

        return false;
    }
#endif

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2020 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Renderers/ChildWindowRenderer.hpp>
#include <TGUI/RendererDefines.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    TGUI_RENDERER_PROPERTY_OUTLINE(ChildWindowRenderer, Borders)

    TGUI_RENDERER_PROPERTY_COLOR(ChildWindowRenderer, TitleBarColor, Color::White)
    TGUI_RENDERER_PROPERTY_COLOR(ChildWindowRenderer, TitleColor, Color::Black)
    TGUI_RENDERER_PROPERTY_COLOR(ChildWindowRenderer, BackgroundColor, Color::White)
    TGUI_RENDERER_PROPERTY_COLOR(ChildWindowRenderer, BorderColor, Color::Black)
    TGUI_RENDERER_PROPERTY_COLOR(ChildWindowRenderer, BorderColorFocused, {})

    TGUI_RENDERER_PROPERTY_TEXTURE(ChildWindowRenderer, TextureTitleBar)
    TGUI_RENDERER_PROPERTY_TEXTURE(ChildWindowRenderer, TextureBackground)

    TGUI_RENDERER_PROPERTY_RENDERER(ChildWindowRenderer, CloseButton, "childwindowbutton")
    TGUI_RENDERER_PROPERTY_RENDERER(ChildWindowRenderer, MaximizeButton, "childwindowbutton")
    TGUI_RENDERER_PROPERTY_RENDERER(ChildWindowRenderer, MinimizeButton, "childwindowbutton")

    TGUI_RENDERER_PROPERTY_NUMBER(ChildWindowRenderer, BorderBelowTitleBar, 0)
    TGUI_RENDERER_PROPERTY_NUMBER(ChildWindowRenderer, DistanceToSide, 3)
    TGUI_RENDERER_PROPERTY_NUMBER(ChildWindowRenderer, PaddingBetweenButtons, 1)
    TGUI_RENDERER_PROPERTY_NUMBER(ChildWindowRenderer, MinimumResizableBorderWidth, 5)

    TGUI_RENDERER_PROPERTY_BOOL(ChildWindowRenderer, ShowTextOnTitleButtons, false)

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float ChildWindowRenderer::getTitleBarHeight() const
    {
        auto it = m_data->propertyValuePairs.find("titlebarheight");
        if (it != m_data->propertyValuePairs.end())
            return it->second.getNumber();
        else
        {
            it = m_data->propertyValuePairs.find("texturetitlebar");
            if (it != m_data->propertyValuePairs.end() && it->second.getTexture().getData())
                return it->second.getTexture().getImageSize().y;
            else
                return 20;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChildWindowRenderer::setTitleBarHeight(float number)
    {
        setProperty("titlebarheight", ObjectConverter{number});
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2020 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets/BoxLayout.hpp>
#include <TGUI/Widgets/ClickableWidget.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    BoxLayout::BoxLayout(const Layout2d& size) :
        Group{size}
    {
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    BoxLayoutRenderer* BoxLayout::getSharedRenderer()
    {
        return aurora::downcast<BoxLayoutRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const BoxLayoutRenderer* BoxLayout::getSharedRenderer() const
    {
        return aurora::downcast<const BoxLayoutRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    BoxLayoutRenderer* BoxLayout::getRenderer()
    {
        return aurora::downcast<BoxLayoutRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const BoxLayoutRenderer* BoxLayout::getRenderer() const
    {
        return aurora::downcast<const BoxLayoutRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BoxLayout::setSize(const Layout2d& size)
    {
        Container::setSize(size);

        updateWidgets();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BoxLayout::add(const Widget::Ptr& widget, const sf::String& widgetName)
    {
        insert(m_widgets.size(), widget, widgetName);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BoxLayout::insert(std::size_t index, const Widget::Ptr& widget, const sf::String& widgetName)
    {
        // Move the widget to the right position
        if (index < m_widgets.size())
        {
            Group::add(widget, widgetName);

            m_widgets.pop_back();

            m_widgets.insert(m_widgets.begin() + index, widget);
        }
        else // Just add the widget to the back
            Group::add(widget, widgetName);

        updateWidgets();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool BoxLayout::remove(const Widget::Ptr& widget)
    {
        for (std::size_t i = 0; i < m_widgets.size(); ++i)
        {
            if (m_widgets[i] == widget)
                return remove(i);
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool BoxLayout::remove(std::size_t index)
    {
        if (index >= m_widgets.size())
            return false;

        Group::remove(m_widgets[index]);

        updateWidgets();
        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Widget::Ptr BoxLayout::get(std::size_t index) const
    {
        if (index < m_widgets.size())
            return m_widgets[index];
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void BoxLayout::rendererChanged(const std::string& property)
    {
        if (property == "spacebetweenwidgets")
        {
            m_spaceBetweenWidgetsCached = getSharedRenderer()->getSpaceBetweenWidgets();
            updateWidgets();
        }
        else if (property == "padding")
        {
            Group::rendererChanged(property);

            // Update the space between widgets as the padding is used when no space was explicitly set
            m_spaceBetweenWidgetsCached = getSharedRenderer()->getSpaceBetweenWidgets();
            updateWidgets();
        }
        else
            Group::rendererChanged(property);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// TGUI - Texus' Graphical User Interface
// Copyright (C) 2012-2020 Bruno Van de Velde (vdv_b@tgui.eu)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <TGUI/Widgets/RadioButtonGroup.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    RadioButtonGroup::RadioButtonGroup()
    {
        m_type = "RadioButtonGroup";
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    RadioButtonGroup::Ptr RadioButtonGroup::create()
    {
        return std::make_shared<RadioButtonGroup>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    RadioButtonGroup::Ptr RadioButtonGroup::copy(RadioButtonGroup::ConstPtr group)
    {
        if (group)
            return std::static_pointer_cast<RadioButtonGroup>(group->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void RadioButtonGroup::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(getPosition());
        drawWidgetContainer(&target, states);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
