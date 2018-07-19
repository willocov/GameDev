#include "Level.h"
#include "graphics.h"
#include "SDL.h"
#include "Globals.h"
#include "tinyxml2.h"
#include <sstream>
#include <algorithm>
#include <cmath>
#include "Utils.h"
#include "Slope.h" 
#include "Player.h"
#include "Enemy.h"

using namespace tinyxml2;

Level::Level(){}
Level::~Level(){}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics &graphics) :
	_mapName(mapName),
	_spawnPoint(spawnPoint),
	_size(Vector2(0,0))
{

	this->loadMap(mapName, graphics);
}

void Level::loadMap(std::string mapName, Graphics &graphics) {
	//Parse the .tmx file
	XMLDocument doc;
	std::stringstream ss;
	ss << "Content/Maps/" << mapName << ".tmx"; //Pass in map 1, we get Content/Maps/Map1.tmx
	doc.LoadFile(ss.str().c_str());

	XMLElement* mapNode = doc.FirstChildElement("map");

	//Get height and width and store it in _size
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height);
	
	//get tile width and tile height 
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_size = Vector2(tileWidth, tileHeight);

	//loading tilesets
	XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	if (pTileset != NULL) {
		while (pTileset) {
			int firstgid;

			//This function loads in the map
			//Must be changed eventually to input new maps
			const char* source;
			if(mapName == "Map5")
				source = "Content/Tilesets/CombinedTileset.png";
			else
				source = "Content/Tilesets/Level2.png";
			char* path; 
			std::stringstream ss;
			ss  <<source;
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
			this->_tilesets.push_back(Tileset(tex, firstgid));

			//Get all animations for that tileset
			XMLElement* pTileA = pTileset->FirstChildElement("tile");
			if (pTileA != NULL) {
				while (pTileA) {
					AnimatedTileInfo ati;
					ati.StartTileId = pTileA->IntAttribute("id") + firstgid;
					ati.TilesetsFirstGid = firstgid;
					XMLElement* pAnimation = pTileA->FirstChildElement("animation");
					if (pAnimation != NULL) {
						while (pAnimation) {
							XMLElement* pFrame = pAnimation->FirstChildElement("frame");
							if (pFrame != NULL) {
								while (pFrame) {
									ati.TileIds.push_back(pFrame->IntAttribute("tileid") + firstgid);
									ati.Duration = pFrame->IntAttribute("duration");
									pFrame = pFrame->NextSiblingElement("frame");
								}
							}
							pAnimation = pAnimation->NextSiblingElement("animation");
						}
					}
					this->_animatedTileInfos.push_back(ati);
					printf("hi\n");
					pTileA = pTileA->NextSiblingElement("tile");
				}
			}
			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}
	
	//Loading the layers
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL){
		while (pLayer) {
			//Loading the data element
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					//loading the tile element
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) {
						int tileCounter = 0;
						while (pTile) {
							//Build each individual tile here
							//if gid = 0, no tile should be drawn. continue loop
							if (pTile->IntAttribute("gid") == 0) {
								tileCounter++;
								if (pTile->NextSiblingElement("tile")){
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else { break; }
							}

							//get tileset for this specific gid
							int gid = pTile->IntAttribute("gid");
							Tileset tls;
							int closest = 0;
							for (int i = 0; i < this->_tilesets.size(); i++) {
								if (this->_tilesets[i].FirstGid <= gid) {
									if (this->_tilesets[i].FirstGid > closest) {
										closest = this->_tilesets[i].FirstGid;
										tls = this->_tilesets.at(i);
									}
								}
							}

							if (tls.FirstGid == -1) {
								//No tileset was found for this gid
								tileCounter++;
								if (pTile->NextSiblingElement("tile")) {
									pTile = pTile->NextSiblingElement("tile");
									continue;
								}
								else { break; }
							}

							//Get position of the tile in the level
							int xx = 0;
							int yy = 0;
							xx = tileCounter % width;
							xx *= tileWidth;
							yy += tileHeight * (tileCounter / width); 
							Vector2 finalTilePosition = Vector2(xx, yy);

							//Calculate position of the tile in the tileset
							Vector2 finalTilesetPosition = this->getTilesetPosition(tls, gid, tileWidth, tileHeight);

							//Build tile and add to level tile list
							bool isAnimatedTile = false;
							AnimatedTileInfo ati;
							for (int i = 0; i < this->_animatedTileInfos.size(); i++) {
								if (this->_animatedTileInfos.at(i).StartTileId == gid) {
									ati = this->_animatedTileInfos.at(i);
									isAnimatedTile = true;
									break;
								}
							}
							if (isAnimatedTile == true) {
								std::vector<Vector2> tilesetPositions;
								for (int i = 0; i < ati.TileIds.size(); i++) {
									tilesetPositions.push_back(this->getTilesetPosition(tls, ati.TileIds.at(i), tileWidth, tileHeight));
									
								}
								AnimatedTile tile(tilesetPositions, ati.Duration, tls.Texture, Vector2(tileWidth, tileHeight), finalTilePosition);
								this->_animatedTileList.push_back(tile);
								printf("hello\n");
							}
							else {
							Tile tile(tls.Texture, Vector2(tileWidth, tileHeight), finalTilesetPosition, finalTilePosition);
							this->_tileList.push_back(tile);
							}

							tileCounter++;
							pTile = pTile->NextSiblingElement("tile");
						}
					}
					pData = pData->NextSiblingElement("data");
				}
			}
			pLayer = pLayer->NextSiblingElement("layer");
		}
	}
	 
	//Parse collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	
	//HARD CODE FOR COLLISION OBJECTS
	//this->_collisionRects.push_back(Rectangle(0.66667 * globals::SPRITE_SCALE, 480 * globals::SPRITE_SCALE, 638 * globals::SPRITE_SCALE, 30 * globals::SPRITE_SCALE));
	
	/*
	this->_collisionRects.push_back(Rectangle(64 * globals::SPRITE_SCALE, 32 * globals::SPRITE_SCALE, 16 * globals::SPRITE_SCALE, 96 * globals::SPRITE_SCALE));
	this->_collisionRects.push_back(Rectangle(80 * globals::SPRITE_SCALE,128 * globals::SPRITE_SCALE,32 * globals::SPRITE_SCALE,16 * globals::SPRITE_SCALE));
	this->_collisionRects.push_back(Rectangle(96 * globals::SPRITE_SCALE,144 * globals::SPRITE_SCALE,16 * globals::SPRITE_SCALE,48 * globals::SPRITE_SCALE));
	this->_collisionRects.push_back(Rectangle(192 * globals::SPRITE_SCALE,144 * globals::SPRITE_SCALE,16 * globals::SPRITE_SCALE,16 * globals::SPRITE_SCALE));
	this->_collisionRects.push_back(Rectangle(208 * globals::SPRITE_SCALE,128 * globals::SPRITE_SCALE,32 * globals::SPRITE_SCALE,16 * globals::SPRITE_SCALE));
	this->_collisionRects.push_back(Rectangle(240 * globals::SPRITE_SCALE,96 * globals::SPRITE_SCALE,16 * globals::SPRITE_SCALE,32 * globals::SPRITE_SCALE));
	this->_collisionRects.push_back(Rectangle(208 * globals::SPRITE_SCALE,160 * globals::SPRITE_SCALE,16 * globals::SPRITE_SCALE,32 * globals::SPRITE_SCALE));
	this->_collisionRects.push_back(Rectangle(112 * globals::SPRITE_SCALE,192 * globals::SPRITE_SCALE,96 * globals::SPRITE_SCALE,16 * globals::SPRITE_SCALE));
	this->_collisionRects.push_back(Rectangle(112 * globals::SPRITE_SCALE,80 * globals::SPRITE_SCALE,144 * globals::SPRITE_SCALE,16 * globals::SPRITE_SCALE));
	this->_collisionRects.push_back(Rectangle(144 * globals::SPRITE_SCALE,64 * globals::SPRITE_SCALE,32 * globals::SPRITE_SCALE,16 * globals::SPRITE_SCALE));
	this->_collisionRects.push_back(Rectangle(64 * globals::SPRITE_SCALE,16 * globals::SPRITE_SCALE,128 * globals::SPRITE_SCALE,16 * globals::SPRITE_SCALE));
	this->_collisionRects.push_back(Rectangle(176 * globals::SPRITE_SCALE,32 * globals::SPRITE_SCALE,16 * globals::SPRITE_SCALE,32 * globals::SPRITE_SCALE));
	this->_collisionRects.push_back(Rectangle(80 * globals::SPRITE_SCALE, 32 * globals::SPRITE_SCALE, 32 * globals::SPRITE_SCALE, 16 * globals::SPRITE_SCALE));
	*/
	//HARD CODE FOR PLAYER SPAWN POINT
	//this->_spawnPoint = Vector2(std::ceil(215) * globals::SPRITE_SCALE, std::ceil(466) * globals::SPRITE_SCALE);

	//HARD CODE FOR THE SLOPES
		//Vector 2 (x1,y1) * sprite scale
		//Vector 2 (x1 + x2, y1 + y2) * sprite scale
	
	/*
	this->_slopes.push_back(Slope(
		Vector2(std::ceil(174.454) * globals::SPRITE_SCALE, std::ceil(193.091) * globals::SPRITE_SCALE),
		Vector2(std::ceil(174.454 + 35.4545) * globals::SPRITE_SCALE, std::ceil(193.091 - 21.4243) * globals::SPRITE_SCALE)
	));
	this->_slopes.push_back(Slope(
		Vector2(std::ceil(111.75) * globals::SPRITE_SCALE, std::ceil(174.625) * globals::SPRITE_SCALE),
		Vector2(std::ceil(111.75 + 36.75) * globals::SPRITE_SCALE, std::ceil(174.625 + 17.25) * globals::SPRITE_SCALE)
	));
	this->_slopes.push_back(Slope(
		Vector2(std::ceil(80) * globals::SPRITE_SCALE, std::ceil(112.75) * globals::SPRITE_SCALE),
		Vector2(std::ceil(80 + 32.8409) * globals::SPRITE_SCALE, std::ceil(112.75 + 15) * globals::SPRITE_SCALE)
	));
	*/

	//Hardcode for enemies
	//this->_enemies.push_back(new Bat(graphics, Vector2(150 * globals::SPRITE_SCALE,125 * globals::SPRITE_SCALE)));
	this->_enemies.push_back(new Bat(graphics, Vector2(70 * globals::SPRITE_SCALE, 170 * globals::SPRITE_SCALE)));


	//PARSING IN COLLISIONS; DOES NOT WORK
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "Collisions") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						this->_collisionRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE
						));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			
			//
			//other objectgroups go here with an else if(ss.str() == "whatever")
			//

			//Parse in slopes
			else if (ss.str() == "Slopes") {
				XMLElement * pObject = pObjectGroup->FirstChildElement("object");
				if(pObject != NULL)
					while (pObject) {
						std::vector<Vector2> points;
						Vector2 p1;
						p1 = Vector2(std::ceil(pObject->FloatAttribute("x")), std::ceil(pObject->FloatAttribute("y")));
						
						XMLElement* pPolyline = pObject->FirstChildElement("polyline");
						if (pPolyline != NULL) {
							std::vector<std::string> pairs;
							const char* pointString = pPolyline->Attribute("points");

							std::stringstream ss;
							ss << pointString;
							Utils::split(ss.str(), pairs, ' ');
							//Now we have each of the pairs. Loop through list of pairs
							//and split them into vector2s and then store them in our points vector
							for (int i = 0; i < pairs.size(); i++) {
								std::vector<std::string> ps;
								Utils::split(pairs.at(i), ps, ',');
								points.push_back(Vector2(std::stoi(ps.at(0)), std::stoi(ps.at(1))));
							}
						}

						for (int i = 0; i < points.size(); i += 2) {
							this->_slopes.push_back(Slope(
								Vector2((p1.x + points.at(i < 2 ? i : i - 1).x) * globals::SPRITE_SCALE,
										(p1.y + points.at(i < 2 ? i : i - 1).y) * globals::SPRITE_SCALE),
								Vector2((p1.x + points.at(i < 2 ? i + 1 : i).x) * globals::SPRITE_SCALE,
										(p1.y + points.at(i < 2 ? i + 1 : i).y) * globals::SPRITE_SCALE)
							));
						}

						pObject = pObject->NextSiblingElement("object");
					}
			}

			//Parse in enemy bats
			else if (ss.str() == "Enemies") {
				float x, y;
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						if (ss.str() == "bat") {
							this->_enemies.push_back(new Bat(graphics, Vector2(std::floor(x) * globals::SPRITE_SCALE, std::floor(y) * globals::SPRITE_SCALE)));
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			/*
			else if (ss.str() == "NextLevel") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");

				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;

						//player spawn point
						if (ss.str() == "next level") {
							this->_spawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE, std::ceil(y) * globals::SPRITE_SCALE);
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}

			}
			*/
			//Parse in spawn points
			else if (ss.str() == "Spawn Point") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				
				if (pObject != NULL) {
					while (pObject) {
						float x = pObject->FloatAttribute("x");
						float y = pObject->FloatAttribute("y");
						const char* name = pObject->Attribute("name");
						std::stringstream ss;
						ss << name;
						
						//player spawn point
						if (ss.str() == "Player Spawn") {
							this->_spawnPoint = Vector2(std::ceil(x) * globals::SPRITE_SCALE, std::ceil(y) * globals::SPRITE_SCALE);
						}
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}

}

void Level::update(int elapsedTime, Player &player) {
	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		this->_animatedTileList.at(i).update(elapsedTime);
}
	for (int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i)->update(elapsedTime, player);
	}
}

std::vector<Rectangle> Level::checkTileCollisions(const Rectangle &other) {
	std::vector<Rectangle> others;
	for (int i = 0; i < this->_collisionRects.size(); i++) {
		if (this->_collisionRects.at(i).collidesWith(other)) {
			others.push_back(this->_collisionRects.at(i));
		}
	}
	return others;
}

void Level::draw(Graphics &graphics) {
	for (int i = 0; i < this->_tileList.size(); i++) {
		this->_tileList.at(i).draw(graphics);
	}
	for (int i = 0; i < this->_animatedTileList.size(); i++) {
		this->_animatedTileList.at(i).draw(graphics);
	}
	for (int i = 0; i < this->_enemies.size(); i++) {
		this->_enemies.at(i)->draw(graphics);
	}

}
	/*Draw the background
	SDL_Rect sourceRect = {0,0,64,64};
	SDL_Rect destRect;

	for (int x = 0; x < this->_size.x / 64; x++) {
		for (int y = 0; y < this->_size.y / 64; y++) {
			destRect.x = x * 64 * globals::SPRITE_SCALE;
			destRect.y = y * 64 * globals::SPRITE_SCALE;
			destRect.w = 64 * globals::SPRITE_SCALE;
			destRect.h = 64 * globals::SPRITE_SCALE;
			graphics.blitSurface(this->_backgroundTexture, &sourceRect, &destRect);
		}
	}*/

std::vector<Slope> Level::checkSlopeCollisions(const Rectangle &other) {
	std::vector<Slope> others;
	for (int i = 0; i < this->_slopes.size(); i++) {
		if (this->_slopes.at(i).collidesWith(other)) {
			others.push_back(this->_slopes.at(i));
		}
	}
	return others;
}

std::vector<Enemy*> Level::checkEnemyCollisions(const Rectangle &other) {
	std::vector<Enemy*> others;
	for (int i = 0; i < this->_enemies.size(); i++) {
		if (this->_enemies.at(i)->getBoundingBox().collidesWith(other)) {
			others.push_back(this->_enemies.at(i));
		}
	}

	return others;
}

const Vector2 Level::getPlayerSpawnPoint() const {
	return this->_spawnPoint;
}

Vector2 Level::getTilesetPosition(Tileset tls, int gid, int tileWidth, int tileHeight) {
	int tilesetWidth, tilesetHeight;
	SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
	int tsxx = gid % (tilesetWidth / tileWidth) - 1;
	tsxx *= tileWidth;
	int tsyy = 0;
	int amt = ((gid - tls.FirstGid) / (tilesetWidth / tileWidth));
	tsyy = tileHeight * amt;
	Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);
	return finalTilesetPosition;
}

