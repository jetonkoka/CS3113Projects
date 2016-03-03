#ifdef _WINDOWS
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <stdio.h>
#ifdef _WINDOWS
#include <GL/glew.h>
#endif
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include "ShaderProgram.h"
#include "Matrix.h"
#include <vector>
#include <string>

#ifdef _WINDOWS
#define RESOURCE_FOLDER ""
#else
#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
#endif

class Entity
{
public:
	void Draw();
	float x;
	float y;
	float rotation;
	int textureID;
	float width;
	float height;
	float speed;
	float direction_x;
	float direction_y;
	bool isAlive;



	Entity(float xPos, float yPos, float rotationVal, int textureId, float Width,
		float Height, float Speed, float Direction_x, float Direction_y, bool moveDown = false) : x(xPos), y(yPos), rotation(rotationVal),
		textureID(textureId), width(Width), height(Height), speed(Speed), direction_x(Direction_x), direction_y(Direction_y), isAlive(moveDown)
	{

	}

	void moveEntity(float x, float y)
	{

		this->x += x;
		this->y += y;
	}
	void scaleEntity(float x, float y)
	{
		this->height *= y;
		this->width *= x;

	}
	void setPos(float x, float y)
	{
		this->x = x;
		this->y = y;

	}



private:




};









class SheetSprite {
public:
	SheetSprite();
	SheetSprite(unsigned int textureIDD, float U, float V, float Width, float Height, float
		Size) : textureID(textureIDD), u(U), v(V), width(Width), height(Height), size(Size)
	{

	}
	void Draw(ShaderProgram *program);
	float size;
	unsigned int textureID;
	float u;
	float v;
	float width;
	float height;

};

class Bullet
{
public:
	float timeAlive;
	float x;
	float y;
	float angle;
	float speed;
	Bullet* shootBullet();
	bool shouldRemoveBullet();
	void update(float elapsed);
	int collide(std::vector<Entity>& theEnemies);
	Bullet(float X, float Y, float Angle=0, float Speed=0.5f) :x(X), y(Y), angle(Angle), speed(Speed)
	{
		
	}
};


int inMenu = 0;
Matrix projectionMatrix;
Matrix projectionMatrixForShip;
Matrix projectionMatrixForBullet;
Matrix projectionMatrixForText;
Matrix modelMatrix;
Matrix modelMatrixForShip;
Matrix modelMatrixForBullet;
Matrix modelMatrixForText;
Matrix viewMatrix;
Matrix viewMatrixForShip;
Matrix viewMatrixForBullet;
Matrix viewMatrixForText;
ShaderProgram *program;
std::vector<Bullet> bullets;











GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
		GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
}





SDL_Window* displayWindow;





Bullet* shootBullet(float x, float y) {
	Bullet *newBullet = new Bullet(x, y);
	return newBullet;
}
bool Bullet::shouldRemoveBullet() {
	if (this->y > 0.75f) {
		return true;
	}
	else {
		return false;
	}
}
int Bullet::collide(std::vector<Entity>& theEnemies)
{
	for (int i = 0; i < theEnemies.size(); i++)
	{
		if (x < (theEnemies[i].x + theEnemies[i].width) && x >(theEnemies[i].x - theEnemies[i].width))
		{
			if (y -y/2< (theEnemies[i].y + .00000001f) || y+y/2 > (theEnemies[i].y -.00000001f) )
			{
				std::cout << "HEIGHT OF BULLET   " << y << std::endl;
				std::cout << "HEIGHT OF enemy" << theEnemies[i].y + theEnemies[i].height << std::endl;
				std::cout << "HEIGHT OF enemy" << theEnemies[i].y - theEnemies[i].height << std::endl;
				return i;
			}
		}
	}
	return 1000;


}

void Bullet::update(float elapsed)
{
	this->timeAlive +=elapsed;
	this->y = this->y + (speed*elapsed);
}














void SheetSprite::Draw(ShaderProgram* program) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	GLfloat texCoords[] = {
		u, v + height,
		u + width, v,
		u, v,
		u + width, v,
		u, v + height,
		u + width, v + height
	};
	float aspect = width / height;
	float vertices[] = {
		(-0.5f) * size * aspect, -0.5f * size,
		(0.5f) * size * aspect, 0.5f * size,
		(-0.5f) * size * aspect, 0.5f * size,
		(0.5f)* size * aspect, 0.5f * size,
		(-0.5f) * size * aspect, -0.5f * size,
		(0.5f)* size * aspect, -0.5f * size };
	// draw our arrays
	glUseProgram(program->programID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

}


void DrawText(ShaderProgram *program, int fontTexture, std::string text, float size, float spacing, float xPosStart, float yPosStart) {
	program->setModelMatrix(modelMatrixForText);
	program->setProjectionMatrix(projectionMatrixForText);
	program->setViewMatrix(viewMatrixForText);
	float texture_size = 1.0 / 16.0f;
	std::vector<float> vertexData;
	std::vector<float> texCoordData;
	for (int i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
		vertexData.insert(vertexData.end(), {
			((size + spacing) * i+xPosStart) + (-0.5f * size ), 0.5f * size +yPosStart,
			((size + spacing) * i+xPosStart) + (-0.5f * size ), -0.5f * size + yPosStart,
			((size + spacing) * i+xPosStart) + (0.5f * size ), 0.5f * size + yPosStart,
			((size + spacing) * i+xPosStart) + (0.5f * size ), -0.5f * size + yPosStart,
			((size + spacing) * i+xPosStart) + (0.5f * size ), 0.5f * size + yPosStart,
			((size + spacing) * i +xPosStart) + (-0.5f * size ), -0.5f * size + yPosStart,
		});
		texCoordData.insert(texCoordData.end(), {
			texture_x, texture_y,
			texture_x, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x + texture_size, texture_y + texture_size,
			texture_x + texture_size, texture_y,
			texture_x, texture_y + texture_size,
		});
	}
	glUseProgram(program->programID);
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertexData.data());
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoordData.data());
	glEnableVertexAttribArray(program->texCoordAttribute);
	glBindTexture(GL_TEXTURE_2D, fontTexture);
	glDrawArrays(GL_TRIANGLES, 0, text.size() * 6);
	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);
}






void update(std::vector<Entity>& theEnemies, float speed, SheetSprite& enemy)
{
	for (int i = 0; i < theEnemies.size(); i++)
	{
		program->setModelMatrix(modelMatrix);

		modelMatrix.identity();
		modelMatrix.Scale(0.25, 0.25, 0);
		modelMatrix.Translate(theEnemies[i].x, theEnemies[i].y, 0);
		if (theEnemies[i].direction_x > 0)
		{
			if (theEnemies[0].x < 1.5f)
			{
				theEnemies[i].moveEntity(3*speed*theEnemies[i].direction_x, 0);
				theEnemies[i].x += speed*theEnemies[i].direction_x;
				modelMatrix.Translate(theEnemies[i].x, 0, 0);

			}
			else 
			{
				theEnemies[i].direction_x *= -1;
				theEnemies[i].y = theEnemies[i].y - theEnemies[i].height;
				modelMatrix.Translate(0, theEnemies[i].y, 0);
			}
		}
		else if (theEnemies[i].direction_x < 0)
		{
			if (theEnemies[theEnemies.size()-1].x > -1.5f)
			{
				theEnemies[i].moveEntity(3*speed*theEnemies[i].direction_x, 0);
				theEnemies[i].x += speed*theEnemies[i].direction_x;
				modelMatrix.Translate(theEnemies[i].x, 0, 0);
			}
			else
			{
				theEnemies[i].direction_x *= -1;
				theEnemies[i].y = theEnemies[i].y - theEnemies[i].height;
				
			}
		}
		program->setModelMatrix(modelMatrix);
		enemy.Draw(program);
	}
}


template<class obj>
void clearTheHeap(std::vector<obj*>& vec)
{
	for (int i = 0; i < vec.size(); i++)
	{
		delete vec[i];
		vec[i] = nullptr;
	}
}






int main(int argc, char *argv[])
{

	//allows me to print to the console. Only affects windows users.
	#ifdef _WINDOWS
		AllocConsole();
		freopen("CONIN$", "r", stdin);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
	#endif



	float lastFrameTicks = 0.0f;
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("SPACE INVADERS!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 360);
	projectionMatrix.setOrthoProjection(-5.33f, 5.33f, -3.0f, 3.0f, -1.0f, 1.0f);

	program = new ShaderProgram(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
	glUseProgram(program->programID);

	SDL_Event event;
	bool done = false;
	
	//starting text
	std::string startingText = "SPACE INVADERS!";
	std::string score = "Score: ";
	std::string pressSpace = "Press enter to start!";
	GLuint textureForText = LoadTexture("font2.png");
	GLuint theEntireSpriteSheet = LoadTexture("sheet.png");


	//the specific textures we need
	SheetSprite enemyBlack = SheetSprite(theEntireSpriteSheet, (423.0f / 1024.0f), 728.0f / 1024.0f, 93.0f / 1024.0f, 84.0f / 1024.0f, 0.50f);
	SheetSprite playerShip = SheetSprite(theEntireSpriteSheet, (211.0f / 1024.0f), 941.0f / 1024.0f, 99.0f / 1024.0f, 75.0f / 1024.0f, 0.15f);
	SheetSprite bullet = SheetSprite(theEntireSpriteSheet, (827.0f / 1024.0f), 125.0f / 1024.0f, 16.0f / 1024.0f, 40.0f / 1024.0f, 0.1f);




	//The Player ship
	Entity player = Entity(-0.90f, -.75f, 0.0f, theEntireSpriteSheet, playerShip.width, playerShip.height, 0, 1, 1);


	//=================================================================================================================================================
	//all the enemies 
	
	Entity enemy2 = Entity(-0.60f, 2.4f, 0.0f, theEntireSpriteSheet, enemyBlack.width*.25, enemyBlack.height*.25, 0, 1, 1);
	Entity enemy3 = Entity(-0.30f, 2.4f, 0.0f, theEntireSpriteSheet, enemyBlack.width*.25, enemyBlack.height*.25, 0, 1, 1);
	Entity enemy6 = Entity(0.0f, 2.4f, 0.0f, theEntireSpriteSheet, enemyBlack.width*.25, enemyBlack.height*.25, 0, 1, 1);
	Entity enemy8 = Entity(0.30f, 2.4f, 0.0f, theEntireSpriteSheet, enemyBlack.width*.25, enemyBlack.height*.25, 0, 1, 1);
	Entity enemy9 = Entity(0.60f, 2.4f, 0.0f, theEntireSpriteSheet, enemyBlack.width*.25, enemyBlack.height*.25, 0, 1, 1);
	Entity enemy = Entity(-0.60f, 1.3f, 0.0f, theEntireSpriteSheet, enemyBlack.width*.25, enemyBlack.height*.25, 0, 1, 1);
	Entity enemy4 = Entity(-0.30f, 1.3f, 0.0f, theEntireSpriteSheet, enemyBlack.width*.25, enemyBlack.height*.25, 0, 1, 1);
	Entity enemy5 = Entity(-0.0f, 1.3f, 0.0f, theEntireSpriteSheet, enemyBlack.width*.25, enemyBlack.height*.25, 0, 1, 1);
	Entity enemy7 = Entity(0.30f, 1.3f, 0.0f, theEntireSpriteSheet, enemyBlack.width*.25, enemyBlack.height*.25, 0, 1, 1);
	Entity enemy10 = Entity(0.60f, 1.3f, 0.0f, theEntireSpriteSheet, enemyBlack.width*.25, enemyBlack.height*.25, 0, 1, 1);

	std::cout << enemyBlack.width << std::endl;
	//===================================================================================================================================================


	//===================================================================================================================================================
	//vector that stores the enemies
	std::vector<Entity> vecOfEnemy;
	std::vector<Entity> vecOfEnemy2;
	vecOfEnemy.push_back(enemy10);
	vecOfEnemy2.push_back(enemy9);
	vecOfEnemy2.push_back(enemy8);
	vecOfEnemy.push_back(enemy7);
	vecOfEnemy2.push_back(enemy6);
	vecOfEnemy.push_back(enemy5);
	vecOfEnemy.push_back(enemy4);
	vecOfEnemy2.push_back(enemy3);
	vecOfEnemy2.push_back(enemy2);
	vecOfEnemy.push_back(enemy);
	//======================================================================================================================================================

	std::vector<Bullet*> theBulletStorage;




	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}

		}
		
		
		const Uint8 *keys = SDL_GetKeyboardState(NULL);
		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		float velocityForShip = 2 * elapsed;
		float velocityForEnemies = 0.05f * elapsed;



		
		glClear(GL_COLOR_BUFFER_BIT);


		//==============================================================================================================================================================



		program->setModelMatrix(modelMatrix);
		program->setProjectionMatrix(projectionMatrix);
		program->setViewMatrix(viewMatrix);

		if (inMenu == 0)
		{
			DrawText(program, textureForText, startingText, .07f, .002f, -.50f, .30f);
			DrawText(program, textureForText, pressSpace, .07f, .0002f, -.7f, -.75f);

		}
		if (keys[SDL_SCANCODE_RETURN])
		{
			++inMenu;

		}
		program->setModelMatrix(modelMatrixForShip);
		program->setProjectionMatrix(projectionMatrixForShip);
		program->setViewMatrix(viewMatrixForShip);
		if (inMenu == 1)
		{
			player.setPos(0, -.75f);
			modelMatrixForShip.setPosition(0, -.75f, 0);
			++inMenu;
			program->setModelMatrix(modelMatrixForBullet);
			program->setProjectionMatrix(projectionMatrixForBullet);
			program->setViewMatrix(viewMatrixForBullet);
			modelMatrix.setPosition(0, -.50f, 0);
		}
	
		playerShip.Draw(program);

		if (inMenu != 0)
		{
			
			update(vecOfEnemy, velocityForEnemies, enemyBlack);
			update(vecOfEnemy2, velocityForEnemies, enemyBlack);
			
			if (keys[SDL_SCANCODE_RIGHT])
			{
				
				if (player.x < .90f && player.direction_x > 0)
				{ 
					
					player.x += velocityForShip;
					modelMatrixForShip.Translate(velocityForShip, 0, 0);

				}
				else
				{
					player.direction_x *= player.direction_x;
				}


				
			}
			else if (keys[SDL_SCANCODE_LEFT])
			{
				if (player.x >-.90f && player.direction_x < 0)
				{
					

					player.moveEntity(-velocityForShip, 0);
					modelMatrixForShip.Translate(-velocityForShip, 0, 0);
				}
				else
				{
					player.direction_x *= -player.direction_x;
				}
				


			}
			playerShip.Draw(program);

			if (keys[SDL_SCANCODE_SPACE])
			{
				if (theBulletStorage.size() == 0)
				{
					Bullet* currBullet = new Bullet(player.x, player.y + 0.1f);
					theBulletStorage.push_back(currBullet);
				
				}
			
			}
			if (theBulletStorage.size() != 0)
			{
				
				if (theBulletStorage[0]->shouldRemoveBullet())
				{
					delete theBulletStorage[0];
					theBulletStorage.clear();

				}
				else
				{
					  
					program->setModelMatrix(modelMatrixForBullet);
					program->setProjectionMatrix(projectionMatrixForBullet);
					program->setViewMatrix(viewMatrixForBullet);
					theBulletStorage[0]->update(elapsed);
					modelMatrixForBullet.setPosition(theBulletStorage[0]->x, theBulletStorage[0]->y, 0);
					bullet.Draw(program);
					int index = theBulletStorage[0]->collide(vecOfEnemy);
					if (index != 1000)
					{
						vecOfEnemy.erase(vecOfEnemy.begin() + index);
						delete theBulletStorage[0];
						theBulletStorage.clear();
					}
					if (theBulletStorage.size() != 0)
					{
						index = theBulletStorage[0]->collide(vecOfEnemy2);
						if (index != 1000)
						{
							vecOfEnemy2.erase(vecOfEnemy2.begin() + index);
							delete theBulletStorage[0];
							theBulletStorage.clear();
						}
					}
					
				}
			}
		

			
			 
		}
		if (inMenu != 0)
		{
			score = "Score: " + std::to_string((-1 * vecOfEnemy.size()-vecOfEnemy2.size() + 10));
			DrawText(program, textureForText, score, .05f, .000002f, -.97f, .68f);
		}
	


		SDL_GL_SwapWindow(displayWindow);



	}

	clearTheHeap(theBulletStorage);
	//delete program;
	//#ifdef _WIN32
	//	std::cin.get();
	//#endif
	//



	SDL_Quit();
	return 0;
}






















































//#ifdef _WIN32
//	std::cin.get();
//#endif
//

//allows me to print to the console. Only affects windows users.
//#ifdef _WINDOWS
//	AllocConsole();
//	freopen("CONIN$", "r", stdin);
//	freopen("CONOUT$", "w", stdout);
//	freopen("CONOUT$", "w", stderr);
//	//===============================================================================================================================================
//#endif
