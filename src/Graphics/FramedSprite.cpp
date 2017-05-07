#include <FRONTIER/Graphics/ShaderManager.hpp>
#include <FRONTIER/Graphics/AttributeRef.hpp>
#include <FRONTIER/Graphics/FramedSprite.hpp>
#include <FRONTIER/Graphics/Attribute.hpp>
#include <FRONTIER/Graphics/Texture.hpp>
#include <FRONTIER/System/Vector4.hpp>

namespace fg
{
    void FramedSprite::buildVertices()
    {
        if (!m_tex) return;

        if (!m_texRect.size.area()) m_texRect.size = m_tex->getRealSize();

        if (!m_size.area()) m_size = m_texRect.size;

        if (m_frameSize.w*2 > m_texRect.size.w) m_frameSize.w = m_texRect.size.w/2;
        if (m_frameSize.h*2 > m_texRect.size.h) m_frameSize.h = m_texRect.size.h/2;

        if (m_size.w < m_frameSize.w*2) m_size.w = m_frameSize.w*2;
        if (m_size.h < m_frameSize.h*2) m_size.h = m_frameSize.h*2;

        fm::vec2 pts[4*4];
        fm::vec2 uvs[4*4];
        fm::vec4 clr[4*4];
        fm::Uint16 inds[9*2*3];

        Cx(4)Cy(4)
        {
            pts[4*x+y] = (fm::vec2(x,y)-fm::vec2i(x,y)/2) * m_frameSize + fm::vec2i(x,y)/2 * (m_size - m_frameSize*2);

            uvs[4*x+y] = (fm::vec2(x,y)-fm::vec2i(x,y)/2) * m_frameSize + fm::vec2i(x,y)/2 * (m_texRect.size - m_frameSize*2) + m_texRect.pos;

            clr[4*x+y] = fm::vec4::White;

            if (x<3 && y<3)
            {
                fm::Size offset = 6*(3*x+y);
				fm::Size difs[] = {0,1,4,4,1,5};
				
				C(sizeof(difs)/sizeof(*difs))
					inds[offset + i] = 4*x + y + difs[i];
            }
        }

        m_draw.reset();

		m_draw.positions    = pts;
        m_draw.texPositions = uvs;
		m_draw.colors       = clr;
        m_draw.addDraw(inds,fg::Triangles);
    }


    /////////////////////////////////////////////////////////////
    FramedSprite::FramedSprite() : m_tex(nullptr)
    {
    	
    }

    /////////////////////////////////////////////////////////////
    FramedSprite::FramedSprite(fm::Ref<const fg::Texture> tex) : m_tex(tex)
    {
        buildVertices();
    }

    /////////////////////////////////////////////////////////////
    FramedSprite::FramedSprite(fm::Ref<const fg::Texture> tex,
                               const fm::rect2s &texRect) : m_tex(tex),
															m_texRect(texRect)
    {
        buildVertices();
    }

    /////////////////////////////////////////////////////////////
    FramedSprite::FramedSprite(fm::Ref<const fg::Texture> tex,
                               const fm::rect2s &texRect,
                               const fm::vec2s &frameSize) : m_tex(tex),
															 m_frameSize(frameSize),
														     m_texRect(texRect)
    {
        buildVertices();
    }
    
    /////////////////////////////////////////////////////////////
    FramedSprite::FramedSprite(const FramedSprite &sprite) : m_tex(sprite.m_tex),
                                                             m_frameSize(sprite.m_frameSize),
                                                             m_texRect(sprite.m_texRect),
                                                             m_size(sprite.m_size),
                                                             m_pos(sprite.m_pos)
    {
        buildVertices();
    }
    
    /////////////////////////////////////////////////////////////
    FramedSprite::FramedSprite(FramedSprite &&sprite)
    {
        sprite.swap(*this);
    }
    
    /////////////////////////////////////////////////////////////
    FramedSprite &FramedSprite::operator=(const FramedSprite &sprite)
    {
        FramedSprite cpy(sprite);
        
        return this->swap(cpy);
    }
    
    /////////////////////////////////////////////////////////////
    FramedSprite &FramedSprite::operator=(FramedSprite &&sprite)
    {
        return this->swap(sprite);
    }
    
    /////////////////////////////////////////////////////////////
    FramedSprite &FramedSprite::swap(FramedSprite &sprite)
    {
        std::swap(m_tex,sprite.m_tex);
        std::swap(m_frameSize,sprite.m_frameSize);
        std::swap(m_texRect,sprite.m_texRect);
        m_draw.swap(sprite.m_draw);
        std::swap(m_size,sprite.m_size);
        std::swap(m_pos,sprite.m_pos);
        
        return *this;
    }

    /////////////////////////////////////////////////////////////
    FramedSprite &FramedSprite::setPosition(const fm::vec2 &pos)
    {
        m_pos = pos;
        return *this;
    }

    /////////////////////////////////////////////////////////////
    const fm::vec2 &FramedSprite::getPosition() const
    {
        return m_pos;
    }


    /////////////////////////////////////////////////////////////
    FramedSprite &FramedSprite::setSize(const fm::vec2 &size)
    {
		if (m_size != size)
		{			
			m_size = size;
			buildVertices();
		}
        return *this;
    }

    /////////////////////////////////////////////////////////////
    const fm::vec2 &FramedSprite::getSize() const
    {
        return m_size;
    }


    /////////////////////////////////////////////////////////////
    FramedSprite &FramedSprite::setTexRect(const fm::rect2s &texRect)
    {
        m_texRect = texRect;
        m_size = texRect.size;
        buildVertices();
        return *this;
    }

    /////////////////////////////////////////////////////////////
    const fm::rect2s &FramedSprite::getTexRect() const
    {
        return m_texRect;
    }


    /////////////////////////////////////////////////////////////
    FramedSprite &FramedSprite::setTexture(fm::Ref<const fg::Texture> tex)
    {
        m_tex = tex;
        setTexRect(fm::rect2s());
        return *this;
    }

    /////////////////////////////////////////////////////////////
    FramedSprite &FramedSprite::setTexture(fm::Ref<const fg::Texture> tex,const fm::rect2s &texRect)
    {
        m_tex = tex;
        setTexRect(texRect);
        return *this;
    }

    /////////////////////////////////////////////////////////////
    const fg::Texture *FramedSprite::getTexture() const
    {
        return m_tex;
    }


    /////////////////////////////////////////////////////////////
    FramedSprite &FramedSprite::setFrameSize(const fm::vec2s &frameSize)
    {
        m_frameSize = frameSize;
        buildVertices();
        return *this;
    }

    /////////////////////////////////////////////////////////////
    const fm::vec2s &FramedSprite::getFrameSize() const
    {
        return m_frameSize;
    }


    /////////////////////////////////////////////////////////////
    void FramedSprite::onDraw(ShaderManager &shader)
    {
        if (!m_tex) return;

        shader.getModelStack().push().mul(fm::MATRIX::translation(fm::vec2i(m_pos)));
        shader.getTexUVStack().push().mul(m_tex->getPixToUnitMatrix());
        shader.useTexture(m_tex);
        shader.draw(m_draw);
        shader.useTexture(nullptr);
        shader.getModelStack().pop();
        shader.getTexUVStack().pop();
    }


    /////////////////////////////////////////////////////////////
    void FramedSprite::onUpdate(const fm::Time &dt)
    {
        (void)dt;
    }
}
