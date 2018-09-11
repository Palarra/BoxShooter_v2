
///* ------------------------------------------------------------------------------------ */
//
///*------------- 1er triangle -------------*/
//circle[0].position = sf::Vector2f(centre.x, centre.y);
//circle[1].position = sf::Vector2f(centre.x, centre.y - texture_size.y / 2);
//circle[2].position = circle[1].position;
//
///*------------- 2eme triangle -------------*/
//circle[3].position = circle[0].position;
//circle[4].position = circle[0].position;
//
///*------------- Texture coordonnées -------------*/
//circle[0].texCoords = sf::Vector2f(texture_size.x / 2, texture_size.y / 2);
//circle[1].texCoords = sf::Vector2f(texture_size.x / 2, 0);
//circle[2].texCoords = circle[1].texCoords;
//
///* ------------------------------------------------------------------------------------ */
//
//
//



/*
switch (quartier)
{
case 1:

circle[2].position = sf::Vector2f(centre.x + pourcentage * texture_size.x / 2, centre.y - texture_size.y / 2);
circle[2].texCoords = sf::Vector2f(texture_size.x / 2 + pourcentage * texture_size.x / 2, 0);

if ( (angle += 2) >= 44.5)
{
quartier++;
circle[2].position = sf::Vector2f(centre.x + texture_size.x / 2, centre.y - texture_size.y / 2);
circle[2].texCoords = sf::Vector2f(texture_size.x / 2 + texture_size.x / 2, 0);
}

break;

case 2:
circle[3].position = sf::Vector2f(centre.x + texture_size.x / 2, centre.y - texture_size.y / 2 + pourcentage * texture_size.y / 2 );
circle[3].texCoords = sf::Vector2f(texture_size.x, pourcentage * texture_size.y / 2);

if ((angle += 2) >= 89.5)
{
quartier++;
circle[3].position = sf::Vector2f(centre.x + texture_size.x / 2, centre.y - texture_size.y / 2 + texture_size.y / 2);
circle[3].texCoords = sf::Vector2f(texture_size.x, texture_size.y / 2);
}
break;

case 3:
circle[4].position = sf::Vector2f(centre.x + texture_size.x / 2, centre.y + pourcentage * texture_size.y / 2);
circle[4].texCoords = sf::Vector2f(texture_size.x, texture_size.y / 2 + pourcentage * texture_size.y / 2);

if ((angle += 2) >= 134.5)
{
quartier++;
circle[4].position = sf::Vector2f(centre.x + texture_size.x / 2, centre.y + texture_size.y / 2);
circle[4].texCoords = sf::Vector2f(texture_size.x, texture_size.y);
}
break;

case 4:
break;

}
*/

/*
if (fmod(angle, 45) < 2 && angle > 0)
{
parties[quartier][2].position = sf::Vector2f(texture_size.x / 2, -1.0f * texture_size.y / 2);
parties[quartier][2].texCoords = sf::Vector2f(texture_size.x, 0);
std::cout << "on ajuste à 45 !!" << std::endl;
}

if (fmod(angle, 90) < 2 && angle > 90)
{
parties[quartier - 1][3].position = sf::Vector2f(texture_size.x / 2, 0);
parties[quartier - 1][3].texCoords = sf::Vector2f(texture_size.x, texture_size.y / 2);
std::cout << "on ajuste à 90 !!" << std::endl;
}*/