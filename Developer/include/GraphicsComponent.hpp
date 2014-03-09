//GraphicsComponent.hpp

#ifndef _GraphicsComponent_hpp_included_
#define _GraphicsComponent_hpp_included_

//Prototypes.
class GameObject;


//============GraphicsComponent=============
//
class GraphicsComponent {
public:

	virtual							~GraphicsComponent();
	virtual void					update(GameObject&, float) = 0;
	virtual void					draw(sf::RenderWindow&) = 0;

	sf::Texture&					getTexture();
	void							setTexture(sf::Texture&);

private:

	sf::Texture						mTexture;

};
//
//==========================================


#endif