//#ifdef _WINDOWS
//	#define _CRT_SECURE_NO_WARNINGS
//#endif
//#include <stdio.h>
//#ifdef _WINDOWS
//	#include <GL/glew.h>
//#endif
//#include <SDL.h>
//#include <SDL_opengl.h>
//#include <SDL_image.h>
//#include "ShaderProgram.h"
//#include "Matrix.h"
//
//
//#ifdef _WINDOWS
//	#define RESOURCE_FOLDER ""
//#else
//	#define RESOURCE_FOLDER "NYUCodebase.app/Contents/Resources/"
//#endif
//
//
//
//GLuint LoadTexture(const char *image_path) {
//	SDL_Surface *surface = IMG_Load(image_path);
//	GLuint textureID;
//	glGenTextures(1, &textureID);
//	glBindTexture(GL_TEXTURE_2D, textureID);
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA,
//		GL_UNSIGNED_BYTE, surface->pixels);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	SDL_FreeSurface(surface);
//	return textureID;
//}
//
//
//
//
//
//SDL_Window* displayWindow;
//
//
//
//
//
//class pongBALL
//{
//public:
//
//	float x;
//	float y;
//	float rotation;
//
//	GLuint textureID;
//
//	float width;
//	float height;
//	
//	float speed;
//	float direction_x;
//	float direction_y;
//
//
//
//	Matrix* modelMatrix;
//	Matrix* projectionMatrix;
//	Matrix* viewMatrix;
//	ShaderProgram* program;
//
//	void draw(GLuint textureID, float width, float height, ShaderProgram *program, Matrix* modelMatrix, Matrix* projectionMatrix, Matrix* viewMatrix)
//	{
//		program->setModelMatrix(*modelMatrix);
//		program->setProjectionMatrix(*projectionMatrix);
//		program->setViewMatrix(*viewMatrix);
//
//
//
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		float vertices[] = { -width, -height, width, -height, width, height, -width, -height, width, height, -width, height };
//		glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
//		glEnableVertexAttribArray(program->positionAttribute);
//		float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
//		glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
//		glEnableVertexAttribArray(program->texCoordAttribute);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		glDisableVertexAttribArray(program->positionAttribute);
//		glDisableVertexAttribArray(program->texCoordAttribute);
//		modelMatrix->setPosition(x, y, 0);
//
//
//	}
//
//	pongBALL(GLuint textureName, ShaderProgram* theProgram, Matrix* model, Matrix* projection, Matrix* view, float xPos, float yPos, float rotate, float widthOfBall, float heightOfBall, float speedOfBall = 1, float directionInX = 1, float directionInY = 1) : x(xPos), y(yPos), rotation(rotate), textureID(textureName), width(widthOfBall), height(heightOfBall), speed(speedOfBall),
//		direction_x(directionInX), direction_y(directionInY), program(theProgram), modelMatrix(model), projectionMatrix(projection), viewMatrix(view)
//	{
//		draw(textureID, widthOfBall, heightOfBall, program, modelMatrix, projectionMatrix, viewMatrix);
//		setPos(program, modelMatrix, projectionMatrix, viewMatrix, xPos, yPos);
//		
//	}
//
//	
//
//	void setPos(ShaderProgram* program, Matrix* modelMatrix, Matrix* projectionMatrix, Matrix* viewMatrix, float x, float y, float z = 0)
//	{
//		program->setModelMatrix(*modelMatrix);
//		program->setProjectionMatrix(*projectionMatrix);
//		program->setViewMatrix(*viewMatrix);
//
//	}
//
//
//
//
//};
//
//
//
//
//int main2(int argc, char *argv[])
//{
//	//allows me to print to the console. Only affects windows users.
//	#ifdef _WINDOWS
//		AllocConsole();
//		freopen("CONIN$", "r", stdin);
//		freopen("CONOUT$", "w", stdout);
//		freopen("CONOUT$", "w", stderr);
//	//===============================================================================================================================================
//	#endif
//
//
//
//
//
//
//	SDL_Init(SDL_INIT_VIDEO);
//	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 360, SDL_WINDOW_OPENGL);
//	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
//	SDL_GL_MakeCurrent(displayWindow, context);
//	#ifdef _WINDOWS
//		glewInit();
//	#endif
//
//	glViewport(0, 0, 640, 360);
//
//	//==================================================================================================================================================
//	ShaderProgram program(RESOURCE_FOLDER"vertex_textured.glsl", RESOURCE_FOLDER"fragment_textured.glsl");
//
//	GLuint leftMostPaddle = LoadTexture("paddle.png");
//	GLuint rightMostPaddle = LoadTexture("paddle.png");
//	GLuint bottomBoundaries = LoadTexture("paddle.png");
//	GLuint topBoundaries = LoadTexture("paddle.png");
//	GLuint middleBoundaries = LoadTexture("middleBoundry.png");
//	GLuint pongBall = LoadTexture("paddle.png");
//
//	Matrix projectionMatrix;
//	Matrix projectionMatrixForRightMostPaddle;
//	Matrix projectionMatrixForBottomBoundaries;
//	Matrix projectionMatrixForTopBoundaries;
//	Matrix projectionMatrixForMiddleBoundaries;
//	Matrix projectionMatrixForBall;
//	Matrix modelMatrix;
//	Matrix ModelMatrixForRightMostPaddle;
//	Matrix modelMatrixForBottomBoundaries;
//	Matrix modelMatrixForTopBoundaries;
//	Matrix modelMatrixForMiddleBoundaries;
//	Matrix modelMatrixForBall;
//	Matrix viewMatrix;
//	Matrix viewMatrixForRightMostPaddle;
//	Matrix viewMatrixForBottomBoundaries;
//	Matrix viewMatrixForTopBoundaries;
//	Matrix viewMatrixForMiddleBoundaries;
//	Matrix viewMatrixForBall;
//
//	//==================================================================================================================================================
//
//	float lastFrameTicks = 0;
//
//
//
//	//coordinates for ball
//	//=================================================================================
//	float xPos = 0;
//	float yPos = -0.5f;
//	float speed = 0;
//	bool lessThan = true;
//	bool firstLoop = true;
//	//=================================================================================
//
//
//
//	
//
//	//attributes for right paddle. Will make this a class if I have time.
//	//==================================================================================
//	float rightPaddlePos = -.18f;
//	float rightPaddleDeltaY = 0.16f;
//	float rightPaddleXPos = .942f;
//	float rightPaddleDeltaX = 0.18f;
//	//==================================================================================
//
//
//	//attributes for left paddle. Will make this a class if I have time.
//	//==================================================================================
//	float leftPaddlePos = -.5;
//	float leftPaddleDeltaY = .5f;
//	float leftPaddleDeltaX = 0.15f;
//	float leftPaddleXPos = -5;
//	//==================================================================================
//
//
//	//attributes for top boundary.
//	//==================================================================================
//	float bottomOfTopBoundary = .975;
//	//==================================================================================
//
//
//
//
//
//
//
//
//	int counter = 0;
//
//	projectionMatrix.setOrthoProjection(-5.33f, 5.33f, -3.0f, 3.0f, -1.0f, 1.0f);
//	
//
//	glUseProgram(program.programID);
//	//float lastFrameTicks = 0.0f;
//
//	SDL_Event event;
//	bool done = false;
//	bool winner;
//
//	while (!done) {
//		while (SDL_PollEvent(&event)) {
//			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
//				done = true;
//			}
//			
//		}
//
//		const Uint8 *keys = SDL_GetKeyboardState(NULL);
//
//		float ticks = (float)SDL_GetTicks() / 1000.0f;
//		float elapsed = ticks - lastFrameTicks;
//		lastFrameTicks = ticks;
//
//		speed = .4 * elapsed;
//
//		
//
//		float speedForPaddle = 3 * elapsed;
//		glClear(GL_COLOR_BUFFER_BIT);
//		
//
//		//==============================================================================================================================================================
//
//
//
//		
//		program.setModelMatrix(modelMatrix);
//		program.setProjectionMatrix(projectionMatrix);
//		program.setViewMatrix(viewMatrix);
//
//
//
//
//
//		//this plots the leftmost paddle.
//		glBindTexture(GL_TEXTURE_2D, leftMostPaddle);
//		float vertices[] = { -.15, -.5, .15, -.5, .15, .5, -.15, -.5, .15, .5, -.15, .5 };
//		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
//		glEnableVertexAttribArray(program.positionAttribute);
//		float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
//		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
//		glEnableVertexAttribArray(program.texCoordAttribute);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		glDisableVertexAttribArray(program.positionAttribute);
//		glDisableVertexAttribArray(program.texCoordAttribute);
//		if (firstLoop)
//		{
//			modelMatrix.setPosition(-5, -.5, 0);
//
//		}
//		
//
//		if (keys[SDL_SCANCODE_W] && leftPaddlePos < 2.4)
//		{
//			modelMatrix.Translate(0, speedForPaddle, 0);
//			leftPaddlePos += speedForPaddle;
//		}
//		else if (keys[SDL_SCANCODE_S] && leftPaddlePos > -2.4)
//		{
//			modelMatrix.Translate(0, -speedForPaddle, 0);
//			leftPaddlePos -= speedForPaddle;
//		}
//
//
//
//
//
//
//		
//		//==============================================================================================================================================================
//		
//
//
//
//		program.setModelMatrix(ModelMatrixForRightMostPaddle);
//		program.setProjectionMatrix(projectionMatrixForRightMostPaddle);
//		program.setViewMatrix(viewMatrixForRightMostPaddle);
//
//
//
//
//
//
//
//		glBindTexture(GL_TEXTURE_2D, rightMostPaddle);
//		float vertices1[] = { -.03, -.16, .03, -.16, .03, .16, -.03, -.16, .03, .16, -.03, .16 };
//		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices1);
//		glEnableVertexAttribArray(program.positionAttribute);
//		float texCoords1[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
//		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords1);
//		glEnableVertexAttribArray(program.texCoordAttribute);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		glDisableVertexAttribArray(program.positionAttribute);
//		glDisableVertexAttribArray(program.texCoordAttribute);
//		if (firstLoop)
//		{
//			ModelMatrixForRightMostPaddle.setPosition(.942, -.18, 0);
//
//		}
//		
//
//		if (keys[SDL_SCANCODE_UP] && rightPaddlePos < .8)
//		{
//			ModelMatrixForRightMostPaddle.Translate(0, speedForPaddle*.35, 0);
//			rightPaddlePos += speedForPaddle*.35;
//		}
//		else if (keys[SDL_SCANCODE_DOWN] && rightPaddlePos > -.8)
//		{
//			ModelMatrixForRightMostPaddle.Translate(0, -speedForPaddle*.35, 0);
//			rightPaddlePos -= speedForPaddle*.35;
//		}
//
//
//
//
//
//
//
//		//==============================================================================================================================================================
//
//
//		program.setModelMatrix(modelMatrixForBottomBoundaries);
//		program.setProjectionMatrix(projectionMatrixForBottomBoundaries);
//		program.setViewMatrix(viewMatrixForBottomBoundaries);
//
//
//
//		glBindTexture(GL_TEXTURE_2D, bottomBoundaries);
//		float vertices2[] = { -1, -.03, 1, -.03, 1, .03, -1, -.03, 1, .03, -1, .03 };
//		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
//		glEnableVertexAttribArray(program.positionAttribute);
//		float texCoords2[] = { 0,1, 1,1, 1,0, 0,1, 1,0, 0,0 };
//		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
//		glEnableVertexAttribArray(program.texCoordAttribute);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		glDisableVertexAttribArray(program.positionAttribute);
//		glDisableVertexAttribArray(program.texCoordAttribute);
//		modelMatrixForBottomBoundaries.setPosition(0, -.97, 0);
//
//
//		program.setModelMatrix(modelMatrixForTopBoundaries);
//		program.setProjectionMatrix(projectionMatrixForTopBoundaries);
//		program.setViewMatrix(viewMatrixForTopBoundaries);
//
//		
//		glBindTexture(GL_TEXTURE_2D, topBoundaries);
//		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices2);
//		glEnableVertexAttribArray(program.positionAttribute);
//		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords2);
//		glEnableVertexAttribArray(program.texCoordAttribute);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		glDisableVertexAttribArray(program.positionAttribute);
//		glDisableVertexAttribArray(program.texCoordAttribute);
//		modelMatrixForTopBoundaries.setPosition(0, .975, 0);
//
//
//
//
//		//==============================================================================================================================================================
//
//
//		program.setModelMatrix(modelMatrixForMiddleBoundaries);
//		program.setProjectionMatrix(projectionMatrixForMiddleBoundaries);
//		program.setViewMatrix(viewMatrixForMiddleBoundaries);
//
//		glBindTexture(GL_TEXTURE_2D, middleBoundaries);
//		float vertices3[] = { -.03, -.945, .03, -.945, .03, .945, -.03, -.945, .03, .945, -.03, .945 };
//		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices3);
//		glEnableVertexAttribArray(program.positionAttribute);
//		float texCoords3[] = { 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0 };
//		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords3);
//		glEnableVertexAttribArray(program.texCoordAttribute);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		glDisableVertexAttribArray(program.positionAttribute);
//		glDisableVertexAttribArray(program.texCoordAttribute);
//		modelMatrixForMiddleBoundaries.setPosition(0, 0, 0);
//
//		
//		//================================================================================================================================================================
//
//		program.setModelMatrix(modelMatrixForBall);
//		program.setProjectionMatrix(projectionMatrixForBall);
//		program.setViewMatrix(viewMatrixForBall);
//
//
//		glBindTexture(GL_TEXTURE_2D, pongBall);
//		
//		float vertices4[] = { -.015, -.03, .015, -.03, .015, .03, -.015, -.03, .015, .03, -.015, .03 };
//		glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices4);
//		glEnableVertexAttribArray(program.positionAttribute);
//		float texCoords4[] = { 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0 };
//		glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords4);
//		glEnableVertexAttribArray(program.texCoordAttribute);
//		glDrawArrays(GL_TRIANGLES, 0, 6);
//		glDisableVertexAttribArray(program.positionAttribute);
//		glDisableVertexAttribArray(program.texCoordAttribute);
//		if (firstLoop)
//		{
//			modelMatrixForBall.setPosition(0, -.5f, 0);
//			modelMatrixForBall.Translate(0, speed, 0);
//		}
//		firstLoop = false;
//
//		
//
//		if (bottomOfTopBoundary - yPos < .11f)
//		{
//			modelMatrixForBall.Translate(0, -speed, 0);
//		}
//		else if (yPos - (-bottomOfTopBoundary) < .02f)
//		{
//			modelMatrixForBall.Translate(0, speed, 0);
//		}
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//		if ((rightPaddleXPos) - xPos > .045f && !firstLoop && lessThan)
//		{
//			
//			modelMatrixForBall.Translate(speed, 0, 0);
//			xPos += speed;
//			//yPos -= speed*.707;
//		}
//		else if ((rightPaddleXPos)-xPos < .045f && ((yPos > rightPaddlePos + rightPaddleDeltaY) || (yPos < rightPaddlePos - rightPaddleDeltaY)) && lessThan)
//		{
//			modelMatrixForBall.Translate(speed, 0, 0);
//			xPos += speed;
//			//yPos -= speed*.707;
//
//		}
//		else if (lessThan)
//		{
//			lessThan = false;
//			modelMatrixForBall.Translate(-speed, 0, 0);
//		}
//
//		if ((xPos - (leftPaddleXPos + leftPaddleDeltaX / 2)) > 4.019 && !firstLoop && !lessThan)
//		{
//			modelMatrixForBall.Translate(-speed , 0, 0);
//			xPos -= speed;
//		
//		}
//		else if (xPos - (leftPaddleXPos + leftPaddleDeltaX / 2) < 4.019 && !((yPos > leftPaddlePos + leftPaddleDeltaY) || (yPos < leftPaddlePos - leftPaddleDeltaY)) && !lessThan)
//		{
//			modelMatrixForBall.Translate(-speed, 0, 0);
//			xPos -= speed;
//		}
//		else if (!lessThan)
//		{
//			lessThan = true;
//			modelMatrixForBall.Translate(speed, 0, 0);
//
//		}
//		if (xPos > 1)
//		{
//			winner = false;
//			break;
//		}
//		else if (xPos < -1 )
//		{
//			winner = true;
//			break;
//
//		}
//		
//
//		
//
//		SDL_GL_SwapWindow(displayWindow);
//
//
//
//	}
//
//	if (winner)
//	{
//		std::cout << "PLAYER 1 WINS!!!!!!!!!!!!!!!!!!!" << std::endl;
//	}
//	else
//	{
//		std::cout << "PLAYER 2 WINS!!!!!!!!!!!!!!!!!!!" << std::endl;
//
//	}
//	#ifdef _WIN32
//		std::cin.get();
//	#endif
//
//
//	SDL_Quit();
//	return 0;
//}
//
//
//
//
//
//
//
//
//
//
///*
//if (firstLoop)
//{
//modelMatrixForBall.setPosition(.25f, -.5f, 0);
////modelMatrixForBall.Translate(speed, speed, 0);
//}
//firstLoop = false;
//if ((rightPaddleXPos - rightPaddleDeltaX / 2) - xPos > .38f && !firstLoop && lessThan)
//{
//modelMatrixForBall.Translate(speed*3, 0, 0);
//xPos += speed;
//}
//else if ((rightPaddleXPos-rightPaddleDeltaX/2) - xPos < .38f && ((yPos > rightPaddlePos + rightPaddleDeltaY) || (yPos < rightPaddlePos - rightPaddleDeltaY)) && lessThan)
//{
//modelMatrixForBall.Translate(speed*3, 0, 0);
//
//}
//else
//{
//lessThan = false;
////modelMatrixForBall.Translate(-speed * 10, 0, 0);
//}
//
//if ((xPos - (leftPaddleXPos + leftPaddleDeltaX / 2)  )&& !firstLoop && !lessThan)
//{
//modelMatrixForBall.Translate(-speed * 3, 0, 0);
//xPos -= speed;
//}
//else if (xPos - (leftPaddleXPos + leftPaddleDeltaX / 2) < 4.8 && ((yPos > leftPaddlePos + leftPaddleDeltaY) || (yPos < leftPaddlePos - leftPaddleDeltaY)) && !lessThan)
//{
//modelMatrixForBall.Translate(-speed * 3, 0, 0);
////std::cout << "WE'RE in HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
//}
//else
//{
//lessThan = true;
//}
//
//
//*/