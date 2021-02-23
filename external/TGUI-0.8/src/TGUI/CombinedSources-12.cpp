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


#include <TGUI/Sprite.hpp>
#include <TGUI/Color.hpp>
#include <TGUI/Clipping.hpp>

#include <cassert>
#include <cmath>

#if TGUI_COMPILED_WITH_CPP_VER >= 17
    #include <optional>
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    Sprite::Sprite(const Texture& texture)
    {
        setTexture(texture);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Sprite::setTexture(const Texture& texture)
    {
        m_texture = texture;
        m_vertexColor = m_texture.getColor();
        m_shader = m_texture.getShader();

        if (isSet())
        {
            if (getSize() == Vector2f{})
                setSize(texture.getImageSize());
            else
                updateVertices();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const Texture& Sprite::getTexture() const
    {
        return m_texture;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef TGUI_NEXT
    Texture& Sprite::getTexture()
    {
        return m_texture;
    }
#endif
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Sprite::isSet() const
    {
        return m_texture.getData() != nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Sprite::setSize(const Vector2f& size)
    {
        m_size.x = std::max(size.x, 0.f);
        m_size.y = std::max(size.y, 0.f);

        if (isSet())
            updateVertices();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f Sprite::getSize() const
    {
        return m_size;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef TGUI_REMOVE_DEPRECATED_CODE
    void Sprite::setColor(const Color& color)
    {
        m_vertexColor = color;

        const Color& vertexColor = Color::calcColorOpacity(m_vertexColor, m_opacity);
        for (auto& vertex : m_vertices)
            vertex.color = vertexColor;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const Color& Sprite::getColor() const
    {
        return m_vertexColor;
    }
#endif
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Sprite::setOpacity(float opacity)
    {
        m_opacity = opacity;

        const Color& vertexColor = Color::calcColorOpacity(m_vertexColor, m_opacity);
        for (auto& vertex : m_vertices)
            vertex.color = vertexColor;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Sprite::getOpacity() const
    {
        return m_opacity;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Sprite::setVisibleRect(const FloatRect& visibleRect)
    {
        m_visibleRect = visibleRect;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    FloatRect Sprite::getVisibleRect() const
    {
        return m_visibleRect;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Sprite::isTransparentPixel(Vector2f pos) const
    {
        if (!isSet() || !m_texture.getData()->image || (m_size.x == 0) || (m_size.y == 0))
            return true;

        if (getRotation() != 0)
        {
            Vector2f offset = {getTransform().transformRect(FloatRect({}, getSize())).left,
                               getTransform().transformRect(FloatRect({}, getSize())).top};

            pos = getInverseTransform().transformPoint(pos) + getInverseTransform().transformPoint(offset);

            // Watch out for rounding errors
            const float epsilon = 0.00001f;
            if ((pos.x < 0) && (pos.x > -epsilon))
                pos.x = 0;
            if ((pos.y < 0) && (pos.y > -epsilon))
                pos.y = 0;
            if ((pos.x >= getSize().x) && (pos.x < getSize().x + epsilon))
                pos.x = getSize().x;
            if ((pos.y >= getSize().y) && (pos.y < getSize().y + epsilon))
                pos.y = getSize().y;
        }
        else // There is no rotation
            pos -= getPosition();

        if ((pos.x < 0) || (pos.y < 0) || (pos.x >= getSize().x) || (pos.y >= getSize().y))
            return true;

        // Find out on which pixel the mouse is standing
        sf::Vector2u pixel;
        sf::IntRect middleRect = m_texture.getMiddleRect();
        const sf::Texture& texture = m_texture.getData()->texture;
        switch (m_scalingType)
        {
            case ScalingType::Normal:
            {
                pixel.x = static_cast<unsigned int>(pos.x / m_size.x * texture.getSize().x);
                pixel.y = static_cast<unsigned int>(pos.y / m_size.y * texture.getSize().y);
                break;
            }
            case ScalingType::Horizontal:
            {
                if (pos.x >= m_size.x - (texture.getSize().x - middleRect.left - middleRect.width) * (m_size.y / texture.getSize().y))
                {
                    float xDiff = (pos.x - (m_size.x - (texture.getSize().x - middleRect.left - middleRect.width) * (m_size.y / texture.getSize().y)));
                    pixel.x = static_cast<unsigned int>(middleRect.left + middleRect.width + (xDiff / m_size.y * texture.getSize().y));
                }
                else if (pos.x >= middleRect.left * (m_size.y / texture.getSize().y))
                {
                    float xDiff = pos.x - (middleRect.left * (m_size.y / texture.getSize().y));
                    pixel.x = static_cast<unsigned int>(middleRect.left + (xDiff / (m_size.x - ((texture.getSize().x - middleRect.width) * (m_size.y / texture.getSize().y))) * middleRect.width));
                }
                else // Mouse on the left part
                {
                    pixel.x = static_cast<unsigned int>(pos.x / m_size.y * texture.getSize().y);
                }

                pixel.y = static_cast<unsigned int>(pos.y / m_size.y * texture.getSize().y);
                break;
            }
            case ScalingType::Vertical:
            {
                if (pos.y >= m_size.y - (texture.getSize().y - middleRect.top - middleRect.height) * (m_size.x / texture.getSize().x))
                {
                    float yDiff = (pos.y - (m_size.y - (texture.getSize().y - middleRect.top - middleRect.height) * (m_size.x / texture.getSize().x)));
                    pixel.y = static_cast<unsigned int>(middleRect.top + middleRect.height + (yDiff / m_size.x * texture.getSize().x));
                }
                else if (pos.y >= middleRect.top * (m_size.x / texture.getSize().x))
                {
                    float yDiff = pos.y - (middleRect.top * (m_size.x / texture.getSize().x));
                    pixel.y = static_cast<unsigned int>(middleRect.top + (yDiff / (m_size.y - ((texture.getSize().y - middleRect.height) * (m_size.x / texture.getSize().x))) * middleRect.height));
                }
                else // Mouse on the top part
                {
                    pixel.y = static_cast<unsigned int>(pos.y / m_size.x * texture.getSize().x);
                }

                pixel.x = static_cast<unsigned int>(pos.x / m_size.x * texture.getSize().x);
                break;
            }
            case ScalingType::NineSlice:
            {
                if (pos.x < middleRect.left)
                    pixel.x = static_cast<unsigned int>(pos.x);
                else if (pos.x >= m_size.x - (texture.getSize().x - middleRect.width - middleRect.left))
                    pixel.x = static_cast<unsigned int>(pos.x - m_size.x + texture.getSize().x);
                else
                {
                    float xDiff = (pos.x - middleRect.left) / (m_size.x - (texture.getSize().x - middleRect.width)) * middleRect.width;
                    pixel.x = static_cast<unsigned int>(middleRect.left + xDiff);
                }

                if (pos.y < middleRect.top)
                    pixel.y = static_cast<unsigned int>(pos.y);
                else if (pos.y >= m_size.y - (texture.getSize().y - middleRect.height - middleRect.top))
                    pixel.y = static_cast<unsigned int>(pos.y - m_size.y + texture.getSize().y);
                else
                {
                    float yDiff = (pos.y - middleRect.top) / (m_size.y - (texture.getSize().y - middleRect.height)) * middleRect.height;
                    pixel.y = static_cast<unsigned int>(middleRect.top + yDiff);
                }

                break;
            }
        };

        return m_texture.isTransparentPixel(pixel);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Sprite::ScalingType Sprite::getScalingType() const
    {
        return m_scalingType;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Sprite::updateVertices()
    {
        // Figure out how the image is scaled best
        Vector2f textureSize;
        FloatRect middleRect;
        if (m_texture.getData()->svgImage)
        {
            if (!m_svgTexture)
                m_svgTexture = aurora::makeCopied<sf::Texture>();

            const sf::Vector2u svgTextureSize{
                static_cast<unsigned int>(std::round(getSize().x)),
                static_cast<unsigned int>(std::round(getSize().y))};

            m_texture.getData()->svgImage->rasterize(*m_svgTexture, svgTextureSize);

            m_scalingType = ScalingType::Normal;
            textureSize = getSize();
        }
        else
        {
            textureSize = m_texture.getImageSize();
            middleRect = sf::FloatRect{m_texture.getMiddleRect()};
            if (middleRect == FloatRect(0, 0, textureSize.x, textureSize.y))
            {
                m_scalingType = ScalingType::Normal;
            }
            else if (middleRect.height == textureSize.y)
            {
                if (m_size.x >= (textureSize.x - middleRect.width) * (m_size.y / textureSize.y))
                    m_scalingType = ScalingType::Horizontal;
                else
                    m_scalingType = ScalingType::Normal;
            }
            else if (middleRect.width == textureSize.x)
            {
                if (m_size.y >= (textureSize.y - middleRect.height) * (m_size.x / textureSize.x))
                    m_scalingType = ScalingType::Vertical;
                else
                    m_scalingType = ScalingType::Normal;
            }
            else
            {
                if (m_size.x >= textureSize.x - middleRect.width)
                {
                    if (m_size.y >= textureSize.y - middleRect.height)
                        m_scalingType = ScalingType::NineSlice;
                    else
                        m_scalingType = ScalingType::Horizontal;
                }
                else if (m_size.y >= (textureSize.y - middleRect.height) * (m_size.x / textureSize.x))
                    m_scalingType = ScalingType::Vertical;
                else
                    m_scalingType = ScalingType::Normal;
            }
        }

        // Calculate the vertices based on the way we are scaling
        const Color& vertexColor = Color::calcColorOpacity(m_vertexColor, m_opacity);
        switch (m_scalingType)
        {
        case ScalingType::Normal:
            ///////////
            // 0---1 //
            // |   | //
            // 2---3 //
            ///////////
            m_vertices.resize(4);
            m_vertices[0] = {{0, 0}, vertexColor, {0, 0}};
            m_vertices[1] = {{m_size.x, 0}, vertexColor, {textureSize.x, 0}};
            m_vertices[2] = {{0, m_size.y}, vertexColor, {0, textureSize.y}};
            m_vertices[3] = {{m_size.x, m_size.y}, vertexColor, {textureSize.x, textureSize.y}};
            break;

        case ScalingType::Horizontal:
            ///////////////////////
            // 0---2-------4---6 //
            // |   |       |   | //
            // 1---3-------5---7 //
            ///////////////////////
            m_vertices.resize(8);
            m_vertices[0] = {{0, 0}, vertexColor, {0, 0}};
            m_vertices[1] = {{0, m_size.y}, vertexColor, {0, textureSize.y}};
            m_vertices[2] = {{middleRect.left * (m_size.y / textureSize.y), 0}, vertexColor, {middleRect.left, 0}};
            m_vertices[3] = {{middleRect.left * (m_size.y / textureSize.y), m_size.y}, vertexColor, {middleRect.left, textureSize.y}};
            m_vertices[4] = {{m_size.x - (textureSize.x - middleRect.left - middleRect.width) * (m_size.y / textureSize.y), 0}, vertexColor, {middleRect.left + middleRect.width, 0}};
            m_vertices[5] = {{m_size.x - (textureSize.x - middleRect.left - middleRect.width) * (m_size.y / textureSize.y), m_size.y}, vertexColor, {middleRect.left + middleRect.width, textureSize.y}};
            m_vertices[6] = {{m_size.x, 0}, vertexColor, {textureSize.x, 0}};
            m_vertices[7] = {{m_size.x, m_size.y}, vertexColor, {textureSize.x, textureSize.y}};
            break;

        case ScalingType::Vertical:
            ///////////
            // 0---1 //
            // |   | //
            // 2---3 //
            // |   | //
            // |   | //
            // |   | //
            // 4---5 //
            // |   | //
            // 6---7-//
            ///////////
            m_vertices.resize(8);
            m_vertices[0] = {{0, 0}, vertexColor, {0, 0}};
            m_vertices[1] = {{m_size.x, 0}, vertexColor, {textureSize.x, 0}};
            m_vertices[2] = {{0, middleRect.top * (m_size.x / textureSize.x)}, vertexColor, {0, middleRect.top}};
            m_vertices[3] = {{m_size.x, middleRect.top * (m_size.x / textureSize.x)}, vertexColor, {textureSize.x, middleRect.top}};
            m_vertices[4] = {{0, m_size.y - (textureSize.y - middleRect.top - middleRect.height) * (m_size.x / textureSize.x)}, vertexColor, {0, middleRect.top + middleRect.height}};
            m_vertices[5] = {{m_size.x, m_size.y - (textureSize.y - middleRect.top - middleRect.height) * (m_size.x / textureSize.x)}, vertexColor, {textureSize.x, middleRect.top + middleRect.height}};
            m_vertices[6] = {{0, m_size.y}, vertexColor, {0, textureSize.y}};
            m_vertices[7] = {{m_size.x, m_size.y}, vertexColor, {textureSize.x, textureSize.y}};
            break;

        case ScalingType::NineSlice:
            //////////////////////////////////
            // 0---1/13-----------14-----15 //
            // |    |              |     |  //
            // 2---3/11----------12/16---17 //
            // |    |              |     |  //
            // |    |              |     |  //
            // |    |              |     |  //
            // 4---5/9-----------10/18---19 //
            // |    |              |     |  //
            // 6----7-------------8/20---21 //
            //////////////////////////////////
            m_vertices.resize(22);
            m_vertices[0] = {{0, 0}, vertexColor, {0, 0}};
            m_vertices[1] = {{middleRect.left, 0}, vertexColor, {middleRect.left, 0}};
            m_vertices[2] = {{0, middleRect.top}, vertexColor, {0, middleRect.top}};
            m_vertices[3] = {{middleRect.left, middleRect.top}, vertexColor, {middleRect.left, middleRect.top}};
            m_vertices[4] = {{0, m_size.y - (textureSize.y - middleRect.top - middleRect.height)}, vertexColor, {0, middleRect.top + middleRect.height}};
            m_vertices[5] = {{middleRect.left, m_size.y - (textureSize.y - middleRect.top - middleRect.height)}, vertexColor, {middleRect.left, middleRect.top + middleRect.height}};
            m_vertices[6] = {{0, m_size.y}, vertexColor, {0, textureSize.y}};
            m_vertices[7] = {{middleRect.left, m_size.y}, vertexColor, {middleRect.left, textureSize.y}};
            m_vertices[8] = {{m_size.x - (textureSize.x - middleRect.left - middleRect.width), m_size.y}, vertexColor, {middleRect.left + middleRect.width, textureSize.y}};
            m_vertices[9] = m_vertices[5];
            m_vertices[10] = {{m_size.x - (textureSize.x - middleRect.left - middleRect.width), m_size.y - (textureSize.y - middleRect.top - middleRect.height)}, vertexColor, {middleRect.left + middleRect.width, middleRect.top + middleRect.height}};
            m_vertices[11] = m_vertices[3];
            m_vertices[12] = {{m_size.x - (textureSize.x - middleRect.left - middleRect.width), middleRect.top}, vertexColor, {middleRect.left + middleRect.width, middleRect.top}};
            m_vertices[13] = m_vertices[1];
            m_vertices[14] = {{m_size.x - (textureSize.x - middleRect.left - middleRect.width), 0}, vertexColor, {middleRect.left + middleRect.width, 0}};
            m_vertices[15] = {{m_size.x, 0}, vertexColor, {textureSize.x, 0}};
            m_vertices[16] = m_vertices[12];
            m_vertices[17] = {{m_size.x, middleRect.top}, vertexColor, {textureSize.x, middleRect.top}};
            m_vertices[18] = m_vertices[10];
            m_vertices[19] = {{m_size.x, m_size.y - (textureSize.y - middleRect.top - middleRect.height)}, vertexColor, {textureSize.x, middleRect.top + middleRect.height}};
            m_vertices[20] = m_vertices[8];
            m_vertices[21] = {{m_size.x, m_size.y}, vertexColor, {textureSize.x, textureSize.y}};
            break;
        };
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Sprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        if (!isSet())
            return;

        // A rotation can cause the image to be shifted, so we move it upfront so that it ends at the correct location
        if (getRotation() != 0)
        {
            Vector2f pos = {getTransform().transformRect(FloatRect({}, getSize())).left,
                            getTransform().transformRect(FloatRect({}, getSize())).top};

            states.transform.translate(getPosition() - pos);
        }

        states.transform *= getTransform();

        // Apply clipping when needed
#if TGUI_COMPILED_WITH_CPP_VER >= 17
        std::optional<Clipping> clipping;
        if (m_visibleRect != FloatRect{})
            clipping.emplace(target, states, Vector2f{m_visibleRect.left, m_visibleRect.top}, Vector2f{m_visibleRect.width, m_visibleRect.height});
#else
        std::unique_ptr<Clipping> clipping;
        if (m_visibleRect != FloatRect{0, 0, 0, 0})
            clipping = std::make_unique<Clipping>(target, states, Vector2f{m_visibleRect.left, m_visibleRect.top}, Vector2f{m_visibleRect.width, m_visibleRect.height});
#endif

        if (m_texture.getData()->svgImage)
            states.texture = m_svgTexture.get();
        else
            states.texture = &m_texture.getData()->texture;

        states.shader = m_shader;
        target.draw(m_vertices.data(), m_vertices.size(), sf::PrimitiveType::TrianglesStrip, states);
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


#include <TGUI/Renderers/KnobRenderer.hpp>
#include <TGUI/RendererDefines.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    TGUI_RENDERER_PROPERTY_OUTLINE(KnobRenderer, Borders)

    TGUI_RENDERER_PROPERTY_COLOR(KnobRenderer, BackgroundColor, Color::White)
    TGUI_RENDERER_PROPERTY_COLOR(KnobRenderer, ThumbColor, Color::Black)
    TGUI_RENDERER_PROPERTY_COLOR(KnobRenderer, BorderColor, Color::Black)

    TGUI_RENDERER_PROPERTY_TEXTURE(KnobRenderer, TextureBackground)
    TGUI_RENDERER_PROPERTY_TEXTURE(KnobRenderer, TextureForeground)

    TGUI_RENDERER_PROPERTY_NUMBER(KnobRenderer, ImageRotation, 0)
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


#include <TGUI/Widgets/ChatBox.hpp>
#include <TGUI/Clipping.hpp>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChatBox::ChatBox()
    {
        m_type = "ChatBox";
        m_draggableWidget = true;

        m_renderer = aurora::makeCopied<ChatBoxRenderer>();
        setRenderer(Theme::getDefault()->getRendererNoThrow(m_type));

        setTextSize(getGlobalTextSize());
        setSize({Text::getLineHeight(m_fontCached, m_textSize) * 18,
                 Text::getLineHeight(m_fontCached, m_textSize) * 8
                 + Text::getExtraVerticalPadding(m_textSize)
                 + m_paddingCached.getTop() + m_paddingCached.getBottom()
                 + m_bordersCached.getTop() + m_bordersCached.getBottom()});
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChatBox::Ptr ChatBox::create()
    {
        return std::make_shared<ChatBox>();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChatBox::Ptr ChatBox::copy(ChatBox::ConstPtr chatBox)
    {
        if (chatBox)
            return std::static_pointer_cast<ChatBox>(chatBox->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChatBoxRenderer* ChatBox::getSharedRenderer()
    {
        return aurora::downcast<ChatBoxRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const ChatBoxRenderer* ChatBox::getSharedRenderer() const
    {
        return aurora::downcast<const ChatBoxRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    ChatBoxRenderer* ChatBox::getRenderer()
    {
        return aurora::downcast<ChatBoxRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const ChatBoxRenderer* ChatBox::getRenderer() const
    {
        return aurora::downcast<const ChatBoxRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::setSize(const Layout2d& size)
    {
        Widget::setSize(size);

        m_spriteBackground.setSize(getInnerSize());

        m_bordersCached.updateParentSize(getSize());
        m_paddingCached.updateParentSize(getSize());

        m_scroll->setPosition(getSize().x - m_bordersCached.getRight() - m_scroll->getSize().x, m_bordersCached.getTop());
        m_scroll->setSize({m_scroll->getSize().x, getInnerSize().y});
        m_scroll->setViewportSize(static_cast<unsigned int>(getInnerSize().y - m_paddingCached.getTop() - m_paddingCached.getBottom()));

        recalculateAllLines();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::addLine(const sf::String& text)
    {
        addLine(text, m_textColor, m_textStyle);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::addLine(const sf::String& text, Color color)
    {
        addLine(text, color, m_textStyle);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::addLine(const sf::String& text, Color color, TextStyle style)
    {
        // Remove the oldest line if you exceed the maximum
        if ((m_maxLines > 0) && (m_maxLines == m_lines.size()))
        {
            if (m_newLinesBelowOthers)
                removeLine(0);
            else
                removeLine(m_maxLines-1);
        }

        Line line;
        line.string = text;
        line.text.setColor(color);
        line.text.setStyle(style);
        line.text.setOpacity(m_opacityCached);
        line.text.setCharacterSize(m_textSize);
        line.text.setString(text);
        line.text.setFont(m_fontCached);

        recalculateLineText(line);

        if (m_newLinesBelowOthers)
            m_lines.push_back(std::move(line));
        else
            m_lines.push_front(std::move(line));

        recalculateFullTextHeight();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    sf::String ChatBox::getLine(std::size_t lineIndex) const
    {
        if (lineIndex < m_lines.size())
        {
            return m_lines[lineIndex].string;
        }
        else // Index too high
            return "";
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Color ChatBox::getLineColor(std::size_t lineIndex) const
    {
        if (lineIndex < m_lines.size())
        {
            return m_lines[lineIndex].text.getColor();
        }
        else // Index too high
            return m_textColor;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TextStyle ChatBox::getLineTextStyle(std::size_t lineIndex) const
    {
        if (lineIndex < m_lines.size())
            return m_lines[lineIndex].text.getStyle();
        else // Index too high
            return m_textStyle;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ChatBox::removeLine(std::size_t lineIndex)
    {
        if (lineIndex < m_lines.size())
        {
            m_lines.erase(m_lines.begin() + lineIndex);

            recalculateFullTextHeight();
            return true;
        }
        else // Index too high
            return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::removeAllLines()
    {
        m_lines.clear();

        recalculateFullTextHeight();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t ChatBox::getLineAmount()
    {
        return m_lines.size();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::setLineLimit(std::size_t maxLines)
    {
        m_maxLines = maxLines;

        // Remove the oldest lines if there are too many lines
        if ((m_maxLines > 0) && (m_maxLines < m_lines.size()))
        {
            if (m_newLinesBelowOthers)
                m_lines.erase(m_lines.begin(), m_lines.begin() + m_lines.size() - m_maxLines);
            else
                m_lines.erase(m_lines.begin() + m_maxLines, m_lines.end());

            recalculateFullTextHeight();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::size_t ChatBox::getLineLimit()
    {
        return m_maxLines;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::setTextSize(unsigned int size)
    {
        m_textSize = size;
        m_scroll->setScrollAmount(size);

        for (auto& line : m_lines)
            line.text.setCharacterSize(size);

        recalculateAllLines();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::setTextColor(Color color)
    {
        m_textColor = color;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const Color& ChatBox::getTextColor() const
    {
        return m_textColor;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::setTextStyle(TextStyle style)
    {
        m_textStyle = style;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    TextStyle ChatBox::getTextStyle() const
    {
        return m_textStyle;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::setLinesStartFromTop(bool startFromTop)
    {
        if (m_linesStartFromTop != startFromTop)
            m_linesStartFromTop = startFromTop;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ChatBox::getLinesStartFromTop() const
    {
        return m_linesStartFromTop;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::setNewLinesBelowOthers(bool newLinesBelowOthers)
    {
        m_newLinesBelowOthers = newLinesBelowOthers;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ChatBox::getNewLinesBelowOthers() const
    {
        return m_newLinesBelowOthers;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::setScrollbarValue(unsigned int value)
    {
        m_scroll->setValue(value);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    unsigned int ChatBox::getScrollbarValue() const
    {
        return m_scroll->getValue();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ChatBox::mouseOnWidget(Vector2f pos) const
    {
        if (FloatRect{getPosition().x, getPosition().y, getSize().x, getSize().y}.contains(pos))
        {
            if (!m_transparentTextureCached || !m_spriteBackground.isTransparentPixel(pos - getPosition() - m_bordersCached.getOffset()))
                return true;
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::leftMousePressed(Vector2f pos)
    {
        // Set the mouse down flag to true
        m_mouseDown = true;

        // Pass the event to the scrollbar
        if (m_scroll->mouseOnWidget(pos - getPosition()))
            m_scroll->leftMousePressed(pos - getPosition());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::leftMouseReleased(Vector2f pos)
    {
        if (m_scroll->isMouseDown())
            m_scroll->leftMouseReleased(pos - getPosition());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::mouseMoved(Vector2f pos)
    {
        if (!m_mouseHover)
            mouseEnteredWidget();

        // Pass the event to the scrollbar when the mouse is on top of it or when we are dragging its thumb
        if (((m_scroll->isMouseDown()) && (m_scroll->isMouseDownOnThumb())) || m_scroll->mouseOnWidget(pos - getPosition()))
            m_scroll->mouseMoved(pos - getPosition());
        else
            m_scroll->mouseNoLongerOnWidget();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::mouseNoLongerOnWidget()
    {
        Widget::mouseNoLongerOnWidget();
        m_scroll->mouseNoLongerOnWidget();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::leftMouseButtonNoLongerDown()
    {
        Widget::leftMouseButtonNoLongerDown();
        m_scroll->leftMouseButtonNoLongerDown();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool ChatBox::mouseWheelScrolled(float delta, Vector2f pos)
    {
        if (m_scroll->getViewportSize() < m_scroll->getMaximum())
        {
            m_scroll->mouseWheelScrolled(delta, pos - getPosition());
            return true;
        }

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::recalculateLineText(Line& line)
    {
        line.text.setString("");

        // Find the maximum width of one line
        const float maxWidth = getInnerSize().x - m_scroll->getSize().x - m_paddingCached.getLeft() - m_paddingCached.getRight();
        if (maxWidth < 0)
            return;

        line.text.setString(Text::wordWrap(maxWidth, line.string, m_fontCached, line.text.getCharacterSize(), false));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::recalculateAllLines()
    {
        for (auto& line : m_lines)
            recalculateLineText(line);

        recalculateFullTextHeight();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::recalculateFullTextHeight()
    {
        m_fullTextHeight = 0;
        for (const auto& line : m_lines)
            m_fullTextHeight += line.text.getSize().y;

        // Update the maximum of the scrollbar
        const unsigned int oldMaximum = m_scroll->getMaximum();
        m_scroll->setMaximum(static_cast<unsigned int>(m_fullTextHeight + Text::getExtraVerticalPadding(m_textSize)));

        // Scroll down to the last item when there is a scrollbar and it is at the bottom
        if (m_newLinesBelowOthers)
        {
            if (((oldMaximum >= m_scroll->getViewportSize()) && (m_scroll->getValue() == oldMaximum - m_scroll->getViewportSize()))
             || ((oldMaximum <= m_scroll->getViewportSize()) && (m_scroll->getMaximum() > m_scroll->getViewportSize())))
            {
                m_scroll->setValue(m_scroll->getMaximum() - m_scroll->getViewportSize());
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::rendererChanged(const std::string& property)
    {
        if (property == "borders")
        {
            m_bordersCached = getSharedRenderer()->getBorders();
            setSize(m_size);
        }
        else if (property == "padding")
        {
            m_paddingCached = getSharedRenderer()->getPadding();
            setSize(m_size);
        }
        else if (property == "texturebackground")
        {
            m_spriteBackground.setTexture(getSharedRenderer()->getTextureBackground());
        }
        else if (property == "scrollbar")
        {
            m_scroll->setRenderer(getSharedRenderer()->getScrollbar());

            // If no scrollbar width was set then we may need to use the one from the texture
            if (!getSharedRenderer()->getScrollbarWidth())
            {
                m_scroll->setSize({m_scroll->getDefaultWidth(), m_scroll->getSize().y});
                setSize(m_size);
            }
        }
        else if (property == "scrollbarwidth")
        {
            const float width = getSharedRenderer()->getScrollbarWidth() ? getSharedRenderer()->getScrollbarWidth() : m_scroll->getDefaultWidth();
            m_scroll->setSize({width, m_scroll->getSize().y});
            setSize(m_size);
        }
        else if (property == "bordercolor")
        {
            m_borderColorCached = getSharedRenderer()->getBorderColor();
        }
        else if (property == "backgroundcolor")
        {
            m_backgroundColorCached = getSharedRenderer()->getBackgroundColor();
        }
        else if ((property == "opacity") || (property == "opacitydisabled"))
        {
            Widget::rendererChanged(property);

            m_spriteBackground.setOpacity(m_opacityCached);
            m_scroll->setInheritedOpacity(m_opacityCached);

            for (auto& line : m_lines)
                line.text.setOpacity(m_opacityCached);
        }
        else if (property == "font")
        {
            Widget::rendererChanged(property);

            for (auto& line : m_lines)
                line.text.setFont(m_fontCached);

            recalculateAllLines();
        }
        else
            Widget::rendererChanged(property);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<DataIO::Node> ChatBox::save(SavingRenderersMap& renderers) const
    {
        auto node = Widget::save(renderers);

        node->propertyValuePairs["TextSize"] = std::make_unique<DataIO::ValueNode>(to_string(m_textSize));
        node->propertyValuePairs["TextColor"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(m_textColor));

        if (m_textStyle != sf::Text::Style::Regular)
            node->propertyValuePairs["TextStyle"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(m_textStyle));

        if (m_maxLines > 0)
            node->propertyValuePairs["LineLimit"] = std::make_unique<DataIO::ValueNode>(to_string(m_maxLines));

        if (m_linesStartFromTop)
            node->propertyValuePairs["LinesStartFromTop"] = std::make_unique<DataIO::ValueNode>("true");
        else
            node->propertyValuePairs["LinesStartFromTop"] = std::make_unique<DataIO::ValueNode>("false");

        if (m_newLinesBelowOthers)
            node->propertyValuePairs["NewLinesBelowOthers"] = std::make_unique<DataIO::ValueNode>("true");
        else
            node->propertyValuePairs["NewLinesBelowOthers"] = std::make_unique<DataIO::ValueNode>("false");

        for (std::size_t i = 0; i < m_lines.size(); ++i)
        {
            auto lineNode = std::make_unique<DataIO::Node>();
            lineNode->name = "Line";

            lineNode->propertyValuePairs["Text"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(getLine(i)));

            const Color lineTextColor = getLineColor(i);
            if (lineTextColor != m_textColor)
                lineNode->propertyValuePairs["Color"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(lineTextColor));

            const TextStyle lineTextStyle = getLineTextStyle(i);
            if (lineTextStyle != m_textStyle)
                lineNode->propertyValuePairs["Style"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(lineTextStyle));

            node->children.push_back(std::move(lineNode));
        }

        return node;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::load(const std::unique_ptr<DataIO::Node>& node, const LoadingRenderersMap& renderers)
    {
        Widget::load(node, renderers);

        if (node->propertyValuePairs["textsize"])
            setTextSize(strToInt(node->propertyValuePairs["textsize"]->value));
        if (node->propertyValuePairs["textcolor"])
            setTextColor(Deserializer::deserialize(ObjectConverter::Type::Color, node->propertyValuePairs["textcolor"]->value).getColor());
        if (node->propertyValuePairs["textstyle"])
            setTextStyle(Deserializer::deserialize(ObjectConverter::Type::TextStyle, node->propertyValuePairs["textstyle"]->value).getTextStyle());
        if (node->propertyValuePairs["linelimit"])
            setLineLimit(strToInt(node->propertyValuePairs["linelimit"]->value));

        for (const auto& childNode : node->children)
        {
            if (toLower(childNode->name) == "line")
            {
                Color lineTextColor = getTextColor();
                if (childNode->propertyValuePairs["color"])
                    lineTextColor = Deserializer::deserialize(ObjectConverter::Type::Color, childNode->propertyValuePairs["color"]->value).getColor();

                TextStyle lineTextStyle = getTextStyle();
                if (childNode->propertyValuePairs["style"])
                    lineTextStyle = Deserializer::deserialize(ObjectConverter::Type::TextStyle, childNode->propertyValuePairs["style"]->value).getTextStyle();

                if (childNode->propertyValuePairs["text"])
                    addLine(Deserializer::deserialize(ObjectConverter::Type::String, childNode->propertyValuePairs["text"]->value).getString(), lineTextColor, lineTextStyle);
            }
        }
        node->children.erase(std::remove_if(node->children.begin(), node->children.end(),
                                        [](const std::unique_ptr<DataIO::Node>& child){ return toLower(child->name) == "line"; }), node->children.end());

        if (node->propertyValuePairs["linesstartfromtop"])
            setLinesStartFromTop(Deserializer::deserialize(ObjectConverter::Type::Bool, node->propertyValuePairs["linesstartfromtop"]->value).getBool());

        // This has to be parsed after the lines have been added
        if (node->propertyValuePairs["newlinesbelowothers"])
            setNewLinesBelowOthers(Deserializer::deserialize(ObjectConverter::Type::Bool, node->propertyValuePairs["newlinesbelowothers"]->value).getBool());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f ChatBox::getInnerSize() const
    {
        return {std::max(0.f, getSize().x - m_bordersCached.getLeft() - m_bordersCached.getRight()),
                std::max(0.f, getSize().y - m_bordersCached.getTop() - m_bordersCached.getBottom())};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void ChatBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(getPosition());

        const sf::RenderStates scrollbarStates = states;

        // Draw the borders
        if (m_bordersCached != Borders{0})
        {
            drawBorders(target, states, m_bordersCached, getSize(), m_borderColorCached);
            states.transform.translate(m_bordersCached.getOffset());
        }

        // Draw the background
        if (m_spriteBackground.isSet())
            m_spriteBackground.draw(target, states);
        else
            drawRectangleShape(target, states, getInnerSize(), m_backgroundColorCached);

        // Draw the scrollbar
        m_scroll->draw(target, scrollbarStates);

        states.transform.translate({m_paddingCached.getLeft(), m_paddingCached.getTop()});

        // Set the clipping for all draw calls that happen until this clipping object goes out of scope
        const Clipping clipping{target, states, {}, {getInnerSize().x - m_paddingCached.getLeft() - m_paddingCached.getRight() - m_scroll->getSize().x,
                                                     getInnerSize().y - m_paddingCached.getTop() - m_paddingCached.getBottom()}};

        states.transform.translate({Text::getExtraHorizontalPadding(m_fontCached, m_textSize), -static_cast<float>(m_scroll->getValue())});

        // Put the lines at the bottom of the chat box if needed
        if (!m_linesStartFromTop && (m_fullTextHeight + Text::getExtraVerticalPadding(m_textSize) < getInnerSize().y - m_paddingCached.getTop() - m_paddingCached.getBottom()))
            states.transform.translate(0, getInnerSize().y - m_paddingCached.getTop() - m_paddingCached.getBottom() - m_fullTextHeight - Text::getExtraVerticalPadding(m_textSize));

        for (const auto& line : m_lines)
        {
            line.text.draw(target, states);
            states.transform.translate(0, line.text.getSize().y);
        }
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


#include <TGUI/Widgets/Slider.hpp>
#include <cmath>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace tgui
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Slider::Slider()
    {
        m_type = "Slider";

        m_draggableWidget = true;

        m_renderer = aurora::makeCopied<SliderRenderer>();
        setRenderer(Theme::getDefault()->getRendererNoThrow(m_type));

        setSize(200, 16);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Slider::Ptr Slider::create(float minimum, float maximum)
    {
        auto slider = std::make_shared<Slider>();

        slider->setMinimum(minimum);
        slider->setMaximum(maximum);

        return slider;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Slider::Ptr Slider::copy(Slider::ConstPtr slider)
    {
        if (slider)
            return std::static_pointer_cast<Slider>(slider->clone());
        else
            return nullptr;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    SliderRenderer* Slider::getSharedRenderer()
    {
        return aurora::downcast<SliderRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const SliderRenderer* Slider::getSharedRenderer() const
    {
        return aurora::downcast<const SliderRenderer*>(Widget::getSharedRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    SliderRenderer* Slider::getRenderer()
    {
        return aurora::downcast<SliderRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const SliderRenderer* Slider::getRenderer() const
    {
        return aurora::downcast<const SliderRenderer*>(Widget::getRenderer());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::setSize(const Layout2d& size)
    {
        Widget::setSize(size);

        m_bordersCached.updateParentSize(getSize());

        if (getSize().x < getSize().y)
            m_verticalScroll = true;
        else if (getSize().x > getSize().y)
            m_verticalScroll = false;

        if (m_spriteTrack.isSet() && m_spriteThumb.isSet())
        {
            float scaleFactor;
            if (m_verticalImage == m_verticalScroll)
            {
                m_spriteTrack.setSize(getInnerSize());
                m_spriteTrackHover.setSize(getInnerSize());

                if (m_verticalScroll)
                    scaleFactor = getInnerSize().x / m_spriteTrack.getTexture().getImageSize().x;
                else
                    scaleFactor = getInnerSize().y / m_spriteTrack.getTexture().getImageSize().y;
            }
            else // The image is rotated
            {
                m_spriteTrack.setSize({getInnerSize().y, getInnerSize().x});
                m_spriteTrackHover.setSize({getInnerSize().y, getInnerSize().x});

                if (m_verticalScroll)
                    scaleFactor = getInnerSize().x / m_spriteTrack.getTexture().getImageSize().y;
                else
                    scaleFactor = getInnerSize().y / m_spriteTrack.getTexture().getImageSize().x;
            }

            m_thumb.width = scaleFactor * m_spriteThumb.getTexture().getImageSize().x;
            m_thumb.height = scaleFactor * m_spriteThumb.getTexture().getImageSize().y;

            m_spriteThumb.setSize({m_thumb.width, m_thumb.height});
            m_spriteThumbHover.setSize({m_thumb.width, m_thumb.height});

            // Apply the rotation now that the size has been set
            if (m_verticalScroll != m_verticalImage)
            {
                m_spriteTrack.setRotation(-90);
                m_spriteTrackHover.setRotation(-90);
                m_spriteThumb.setRotation(-90);
                m_spriteThumbHover.setRotation(-90);
            }
            else
            {
                m_spriteTrack.setRotation(0);
                m_spriteTrackHover.setRotation(0);
                m_spriteThumb.setRotation(0);
                m_spriteThumbHover.setRotation(0);
            }
        }
        else // There are no textures
        {
            if (m_verticalScroll)
            {
                m_thumb.width = getSize().x * 1.6f;
                m_thumb.height = m_thumb.width / 2.0f;
            }
            else
            {
                m_thumb.height = getSize().y * 1.6f;
                m_thumb.width = m_thumb.height / 2.0f;
            }
        }

        updateThumbPosition();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f Slider::getFullSize() const
    {
        if (m_verticalScroll)
            return {std::max(getSize().x, m_thumb.width), getSize().y + m_thumb.height};
        else
            return {getSize().x + m_thumb.width, std::max(getSize().y, m_thumb.height)};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f Slider::getWidgetOffset() const
    {
        if (m_verticalScroll)
            return {std::min(0.f, (getSize().x - m_thumb.width) / 2.f), -m_thumb.height / 2.f};
        else
            return {-m_thumb.width / 2.f, std::min(0.f, (getSize().y - m_thumb.height) / 2.f)};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::setMinimum(float minimum)
    {
        // Set the new minimum
        m_minimum = minimum;

        // The maximum can't be below the minimum
        if (m_maximum < m_minimum)
            m_maximum = m_minimum;

        // When the value is below the minimum then adjust it
        if (m_value < m_minimum)
            setValue(m_minimum);

        updateThumbPosition();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Slider::getMinimum() const
    {
        return m_minimum;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::setMaximum(float maximum)
    {
        // Set the new maximum
        m_maximum = maximum;

        // The minimum can't be below the maximum
        if (m_minimum > m_maximum)
            setMinimum(m_maximum);

        // When the value is above the maximum then adjust it
        if (m_value > m_maximum)
            setValue(m_maximum);

        updateThumbPosition();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Slider::getMaximum() const
    {
        return m_maximum;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::setValue(float value)
    {
        // Round to nearest allowed value
        if (m_step != 0)
           value = m_minimum + (std::round((value - m_minimum) / m_step) * m_step);

        // When the value is below the minimum or above the maximum then adjust it
        if (value < m_minimum)
            value = m_minimum;
        else if (value > m_maximum)
            value = m_maximum;

        if (m_value != value)
        {
            m_value = value;

            onValueChange.emit(this, m_value);

            updateThumbPosition();
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Slider::getValue() const
    {
        return m_value;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::setStep(float step)
    {
        m_step = step;

        // Reset the value in case it does not match the step
        setValue(m_value);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    float Slider::getStep() const
    {
        return m_step;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::setVerticalScroll(bool vertical)
    {
        if (m_verticalScroll == vertical)
            return;

        m_verticalScroll = vertical;
        setSize(getSize().y, getSize().x);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Slider::getVerticalScroll() const
    {
        return m_verticalScroll;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::setInvertedDirection(bool invertedDirection)
    {
        m_invertedDirection = invertedDirection;
        updateThumbPosition();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Slider::getInvertedDirection() const
    {
        return m_invertedDirection;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::setChangeValueOnScroll(bool changeValueOnScroll)
    {
        m_changeValueOnScroll = changeValueOnScroll;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Slider::getChangeValueOnScroll() const
    {
        return m_changeValueOnScroll;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Slider::mouseOnWidget(Vector2f pos) const
    {
        pos -= getPosition();

        // Check if the mouse is on top of the thumb
        if (FloatRect(m_thumb.left, m_thumb.top, m_thumb.width, m_thumb.height).contains(pos))
            return true;

        // Check if the mouse is on top of the track
        if (FloatRect{0, 0, getSize().x, getSize().y}.contains(pos))
            return true;

        return false;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::leftMousePressed(Vector2f pos)
    {
        m_mouseDown = true;

        if (FloatRect(m_thumb.left, m_thumb.top, m_thumb.width, m_thumb.height).contains(pos))
        {
            m_mouseDownOnThumb = true;
            m_mouseDownOnThumbPos.x = pos.x - m_thumb.left;
            m_mouseDownOnThumbPos.y = pos.y - m_thumb.top;
        }
        else // The mouse is not on top of the thumb
            m_mouseDownOnThumb = false;

        // Refresh the value
        mouseMoved(pos);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::leftMouseReleased(Vector2f)
    {
        // The thumb might have been dragged between two values
        if (m_mouseDown)
            updateThumbPosition();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::mouseMoved(Vector2f pos)
    {
        pos -= getPosition();

        if (!m_mouseHover)
            mouseEnteredWidget();

        if (!m_mouseDown)
            return;

        // Check in which direction the slider goes
        if (m_verticalScroll)
        {
            // Check if the click occurred on the track
            if (!m_mouseDownOnThumb)
            {
                m_mouseDownOnThumb = true;
                m_mouseDownOnThumbPos.x = pos.x - m_thumb.left;
                m_mouseDownOnThumbPos.y = m_thumb.height / 2.0f;
            }

            float value = m_maximum - (((pos.y + (m_thumb.height / 2.0f) - m_mouseDownOnThumbPos.y) / getSize().y) * (m_maximum - m_minimum));
            if (m_invertedDirection)
                value = m_maximum - (value - m_minimum);

            setValue(value);

            // Set the thumb position for smooth scrolling
            const float thumbTop = pos.y - m_mouseDownOnThumbPos.y;
            if ((thumbTop + (m_thumb.height / 2.0f) > 0) && (thumbTop + (m_thumb.height / 2.0f) < getSize().y))
                m_thumb.top = thumbTop;
            else
            {
                m_thumb.top = (getSize().y / (m_maximum - m_minimum) * (m_maximum - m_value)) - (m_thumb.height / 2.0f);
                if (m_invertedDirection)
                    m_thumb.top = getSize().y - m_thumb.top - m_thumb.height;
            }
        }
        else // the slider lies horizontal
        {
            // Check if the click occurred on the track
            if (!m_mouseDownOnThumb)
            {
                m_mouseDownOnThumb = true;
                m_mouseDownOnThumbPos.x = m_thumb.width / 2.0f;
                m_mouseDownOnThumbPos.y = pos.y - m_thumb.top;
            }

            float value = (((pos.x + (m_thumb.width / 2.0f) - m_mouseDownOnThumbPos.x) / getSize().x) * (m_maximum - m_minimum)) + m_minimum;
            if (m_invertedDirection)
                value = m_maximum - (value - m_minimum);

            setValue(value);

            // Set the thumb position for smooth scrolling
            const float thumbLeft = pos.x - m_mouseDownOnThumbPos.x;
            if ((thumbLeft + (m_thumb.width / 2.0f) > 0) && (thumbLeft + (m_thumb.width / 2.0f) < getSize().x))
                m_thumb.left = thumbLeft;
            else
            {
                m_thumb.left = (getSize().x / (m_maximum - m_minimum) * (m_value - m_minimum)) - (m_thumb.width / 2.0f);
                if (m_invertedDirection)
                    m_thumb.left = getSize().x - m_thumb.left - m_thumb.width;
            }
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    bool Slider::mouseWheelScrolled(float delta, Vector2f)
    {
        // Don't do anything when changing value on scroll is disabled
        if (!m_changeValueOnScroll)
            return false;

        if (m_invertedDirection)
            delta = -delta;

        if (m_step == 0)
            setValue(m_value + delta);
        else
        {
            if (std::abs(delta) <= 1)
            {
                if (delta > 0)
                    setValue(m_value + m_step);
                else if (delta < 0)
                    setValue(m_value - m_step);
            }
            else
                setValue(m_value + std::round(delta) * m_step);
        }

        return true;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::leftMouseButtonNoLongerDown()
    {
        // The thumb might have been dragged between two values
        if (m_mouseDown)
            updateThumbPosition();

        Widget::leftMouseButtonNoLongerDown();
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Signal& Slider::getSignal(std::string signalName)
    {
        if (signalName == toLower(onValueChange.getName()))
            return onValueChange;
        else
            return Widget::getSignal(std::move(signalName));
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::rendererChanged(const std::string& property)
    {
        if (property == "borders")
        {
            m_bordersCached = getSharedRenderer()->getBorders();
            setSize(m_size);
        }
        else if (property == "texturetrack")
        {
            m_spriteTrack.setTexture(getSharedRenderer()->getTextureTrack());

            if (m_spriteTrack.getTexture().getImageSize().x < m_spriteTrack.getTexture().getImageSize().y)
                m_verticalImage = true;
            else
                m_verticalImage = false;

            setSize(m_size);
        }
        else if (property == "texturetrackhover")
        {
            m_spriteTrackHover.setTexture(getSharedRenderer()->getTextureTrackHover());
        }
        else if (property == "texturethumb")
        {
            m_spriteThumb.setTexture(getSharedRenderer()->getTextureThumb());
            setSize(m_size);
        }
        else if (property == "texturethumbhover")
        {
            m_spriteThumbHover.setTexture(getSharedRenderer()->getTextureThumbHover());
        }
        else if (property == "trackcolor")
        {
            m_trackColorCached = getSharedRenderer()->getTrackColor();
        }
        else if (property == "trackcolorhover")
        {
            m_trackColorHoverCached = getSharedRenderer()->getTrackColorHover();
        }
        else if (property == "thumbcolor")
        {
            m_thumbColorCached = getSharedRenderer()->getThumbColor();
        }
        else if (property == "thumbcolorhover")
        {
            m_thumbColorHoverCached = getSharedRenderer()->getThumbColorHover();
        }
        else if (property == "bordercolor")
        {
            m_borderColorCached = getSharedRenderer()->getBorderColor();
        }
        else if (property == "bordercolorhover")
        {
            m_borderColorHoverCached = getSharedRenderer()->getBorderColorHover();
        }
        else if ((property == "opacity") || (property == "opacitydisabled"))
        {
            Widget::rendererChanged(property);

            m_spriteTrack.setOpacity(m_opacityCached);
            m_spriteTrackHover.setOpacity(m_opacityCached);
            m_spriteThumb.setOpacity(m_opacityCached);
            m_spriteThumbHover.setOpacity(m_opacityCached);
        }
        else
            Widget::rendererChanged(property);
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    std::unique_ptr<DataIO::Node> Slider::save(SavingRenderersMap& renderers) const
    {
        auto node = Widget::save(renderers);
        node->propertyValuePairs["Minimum"] = std::make_unique<DataIO::ValueNode>(to_string(m_minimum));
        node->propertyValuePairs["Maximum"] = std::make_unique<DataIO::ValueNode>(to_string(m_maximum));
        node->propertyValuePairs["Value"] = std::make_unique<DataIO::ValueNode>(to_string(m_value));
        node->propertyValuePairs["Step"] = std::make_unique<DataIO::ValueNode>(to_string(m_step));
        node->propertyValuePairs["InvertedDirection"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(m_invertedDirection));
        node->propertyValuePairs["ChangeValueOnScroll"] = std::make_unique<DataIO::ValueNode>(Serializer::serialize(m_changeValueOnScroll));
        return node;
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::load(const std::unique_ptr<DataIO::Node>& node, const LoadingRenderersMap& renderers)
    {
        Widget::load(node, renderers);

        if (node->propertyValuePairs["minimum"])
            setMinimum(strToFloat(node->propertyValuePairs["minimum"]->value));
        if (node->propertyValuePairs["maximum"])
            setMaximum(strToFloat(node->propertyValuePairs["maximum"]->value));
        if (node->propertyValuePairs["value"])
            setValue(strToFloat(node->propertyValuePairs["value"]->value));
        if (node->propertyValuePairs["step"])
            setStep(strToFloat(node->propertyValuePairs["step"]->value));
        if (node->propertyValuePairs["inverteddirection"])
            setInvertedDirection(Deserializer::deserialize(ObjectConverter::Type::Bool, node->propertyValuePairs["inverteddirection"]->value).getBool());
        if (node->propertyValuePairs["changevalueonscroll"])
            setChangeValueOnScroll(Deserializer::deserialize(ObjectConverter::Type::Bool, node->propertyValuePairs["changevalueonscroll"]->value).getBool());
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    Vector2f Slider::getInnerSize() const
    {
        return {std::max(0.f, getSize().x - m_bordersCached.getLeft() - m_bordersCached.getRight()),
                std::max(0.f, getSize().y - m_bordersCached.getTop() - m_bordersCached.getBottom())};
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::updateThumbPosition()
    {
        if (m_verticalScroll)
        {
            m_thumb.left = m_bordersCached.getLeft() + (getInnerSize().x - m_thumb.width) / 2.0f;
            m_thumb.top = (getSize().y / (m_maximum - m_minimum) * (m_maximum - m_value)) - (m_thumb.height / 2.0f);

            if (m_invertedDirection)
                m_thumb.top = getSize().y - m_thumb.top - m_thumb.height;
        }
        else
        {
            m_thumb.left = (getSize().x / (m_maximum - m_minimum) * (m_value - m_minimum)) - (m_thumb.width / 2.0f);
            m_thumb.top = m_bordersCached.getTop() + (getInnerSize().y - m_thumb.height) / 2.0f;

            if (m_invertedDirection)
                m_thumb.left = getSize().x - m_thumb.left - m_thumb.width;
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    void Slider::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform.translate(getPosition());

        // Draw the borders around the track
        if (m_bordersCached != Borders{0})
        {
            if (m_mouseHover && m_borderColorHoverCached.isSet())
                drawBorders(target, states, m_bordersCached, getSize(), m_borderColorHoverCached);
            else
                drawBorders(target, states, m_bordersCached, getSize(), m_borderColorCached);

            states.transform.translate({m_bordersCached.getLeft(), m_bordersCached.getTop()});
        }

        // Draw the track
        if (m_spriteTrack.isSet() && m_spriteThumb.isSet())
        {
            if (m_mouseHover && m_spriteTrackHover.isSet())
                m_spriteTrackHover.draw(target, states);
            else
                m_spriteTrack.draw(target, states);
        }
        else // There are no textures
        {
            if (m_mouseHover && m_trackColorHoverCached.isSet())
                drawRectangleShape(target, states, getInnerSize(), m_trackColorHoverCached);
            else
                drawRectangleShape(target, states, getInnerSize(), m_trackColorCached);
        }

        states.transform.translate({-m_bordersCached.getLeft() + m_thumb.left, -m_bordersCached.getTop() + m_thumb.top});

        // Draw the borders around the thumb when using colors
        if ((m_bordersCached != Borders{0}) && !(m_spriteTrack.isSet() && m_spriteThumb.isSet()))
        {
            if (m_mouseHover && m_borderColorHoverCached.isSet())
                drawBorders(target, states, m_bordersCached, {m_thumb.width, m_thumb.height}, m_borderColorHoverCached);
            else
                drawBorders(target, states, m_bordersCached, {m_thumb.width, m_thumb.height}, m_borderColorCached);

            states.transform.translate({m_bordersCached.getLeft(), m_bordersCached.getTop()});
        }

        // Draw the thumb
        if (m_spriteTrack.isSet() && m_spriteThumb.isSet())
        {
            if (m_mouseHover && m_spriteThumbHover.isSet())
                m_spriteThumbHover.draw(target, states);
            else
                m_spriteThumb.draw(target, states);
        }
        else // There are no textures
        {
            const Vector2f thumbInnerSize = {m_thumb.width - m_bordersCached.getLeft() - m_bordersCached.getRight(),
                                             m_thumb.height - m_bordersCached.getTop() - m_bordersCached.getBottom()};

            if (m_mouseHover && m_thumbColorHoverCached.isSet())
                drawRectangleShape(target, states, thumbInnerSize, m_thumbColorHoverCached);
            else
                drawRectangleShape(target, states, thumbInnerSize, m_thumbColorCached);
        }
    }

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
