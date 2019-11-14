#include <string>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <AntTweakBar.h>

#include "Shader.h"
//#include "Camera.h"
//使用自己的 camera 
#include "camera_mine.h"
#include "Model.h"
//#include "model_mine.h"
//#include "model_mine.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <SOIL.h>
//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"
#include <iostream>

GLuint screenWidth = 1200, screenHeight = 800;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
unsigned int loadTexture(const char *path);
unsigned int loadCubemap(vector<std::string> faces);
//整合
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// CAMERA
//Camera camera(12.5f, 0.45f);
//bool keys[1024];
//GLfloat lastX = 400, lastY = 300;
//bool firstMouse = true;

//camera_mine
// 基础设置
const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
// 我的照相机
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;


namespace {
	double gCursorPositionY = 0.0;

	void cursorPositionCallback(GLFWwindow *const window,
		const double xpos, const double ypos)
	{
		gCursorPositionY = ypos;
		if (!TwEventMousePosGLFW(static_cast<int>(xpos), static_cast<int>(ypos)))
		{
			mouse_callback(window, xpos, ypos);
		}
	}

	void keyCallback(GLFWwindow *const window, const int key,
		const int scancode, const int action, const int mods)
	{
		if (TwEventKeyGLFW(key, action) == 0)
		{
			// gApplication.onKey(key, scancode, action, mods);
		}
	}

	/*void scrollCallback(GLFWwindow *const window,
		const double xoffset, const double yoffset)
	{
		if (!TwEventMouseWheelGLFW(static_cast<int>(gCursorPositionY)))
		{
			camera.updateZoom(yoffset);
		}
	}*/
	// ----------------------------------------------------------------------
	/*void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		camera.ProcessMouseScroll(yoffset);
	}*/

	void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		if (!TwEventMouseButtonGLFW(button, action))
		{
			// gApplication.onMouseButton(button, action, mods);
		}
	}

}

int main()
{
	// GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);	
	glfwWindowHint(GLFW_SAMPLES, 4);

	//glfwSwapInterval(1);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "B17040425_ShadowVolume", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	// 捕获鼠标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	glViewport(0, 0, screenWidth, screenHeight);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	// SHADER
	Shader renderShader("./shader/render.vert", "./shader/render.frag");
	Shader volumeShader("./shader/volume.vert", "./shader/volume.frag", "./shader/volume.geom");
	Shader compShader("./shader/comp.vert", "./shader/comp.frag");
	Shader planeShader("./shader/plane.vert", "./shader/plane.frag");
	Shader skyboxShader("./shader/skybox.vert", "./shader/skybox.frag");
	Shader ourShader("model_loading.vs", "model_loading.fs");
	//测试
	Model ourModel("C:/Users/周奕飞/source/repos/GHomeworkThreePre/GHomeworkThreePre/images/ball.obj");

	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};

	// skybox VAO
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// load textures
	// -------------
	vector<std::string> faces
	{
		"./cube/right.jpg",
		"./cube/left.jpg",
		"./cube/top.jpg",
		"./cube/bottom.jpg",
		"./cube/back.jpg",
		"./cube/front.jpg"
	};
	unsigned int cubemapTexture = loadCubemap(faces);

	// MODELLO
	//Model millModel("./Deer/Deer.obj");
	Model millModel("./Deer/Ball.obj");


	// DEPTH BUFFER
	GLuint depthBuf;
	glGenRenderbuffers(1, &depthBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, screenWidth, screenHeight);

	// AMBIENT BUFFER
	GLuint ambBuf;
	glGenRenderbuffers(1, &ambBuf);
	glBindRenderbuffer(GL_RENDERBUFFER, ambBuf);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA, screenWidth, screenHeight);

	// DIFFUSE COMPONENT
	GLuint diffSpecTex;
	glGenTextures(1, &diffSpecTex);
	glBindTexture(GL_TEXTURE_2D, diffSpecTex);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, screenWidth, screenHeight);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	GLuint colorDepthFBO = 0;

	// FBO
	glGenFramebuffers(1, &colorDepthFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, colorDepthFBO);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, ambBuf);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, diffSpecTex, 0);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, drawBuffers);

	GLenum result = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (result == GL_FRAMEBUFFER_COMPLETE) {
		printf("Framebuffer is complete.\n");
	}
	else {
		printf("Framebuffer is not complete.\n");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// VAO SCHERMO
	GLfloat verts[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f };
	GLuint bufHandle;
	glGenBuffers(1, &bufHandle);
	glBindBuffer(GL_ARRAY_BUFFER, bufHandle);
	glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(GLfloat), verts, GL_STATIC_DRAW);

	GLuint fsQuad = 1;
	glGenVertexArrays(1, &fsQuad);
	glBindVertexArray(fsQuad);

	glBindBuffer(GL_ARRAY_BUFFER, bufHandle);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	float angle = 0.0f;

	glm::vec4 lightPosition = glm::vec4(100.5, 0.1f, 0.0f, 1.0f);

	glm::mat4 model;
	model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
	model = glm::rotate(model, 1.1f, glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::translate(model, glm::vec3(-10.0f, 4.3f, 20.0f));

	//PIANO
	GLfloat plainVertices[] = {
		15.0f, 0.0f, 15.0f,
		-15.0f, 0.0f, 15.0f,
		15.0f, 0.0f, -15.0f,
		-15.0f, 0.0f, -15.0f
	};

	GLuint VBO[1], VAO[1];
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, VBO);

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(plainVertices), plainVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	//
	// Initialize AntTweakBar
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(screenWidth, screenHeight);
	// Create a tweak bar
	TwBar *bar;
	float speed = 1.0f;
	float height = 1.75f;
	float distance = 4.0f;
	float intensity = 1.0f;
	int showEdge = 0;
	int showFrustum = 0;
	float time = 0.0f;
	float fps = 60.0f;
	bar = TwNewBar("ShadowVolumeOption");
	TwAddVarRW(bar, "Show Edge", TW_TYPE_BOOL32, &showEdge, "label='Show Edge' group=Geometry");
	TwAddVarRW(bar, "Show Frustum", TW_TYPE_BOOL32, &showFrustum, "label='Show Frustum' group=Geometry");

	TwAddVarRW(bar, "Light Orbit Distance", TW_TYPE_FLOAT, &distance, "label='Light Orbit Distance' min=0.1 max=10 step=0.02 group=Lighting");
	TwAddVarRW(bar, "Light Height", TW_TYPE_FLOAT, &height, "label='Light Height' min=0.1 max=5 step=0.02 group=Lighting");
	TwAddVarRW(bar, "Light Speed", TW_TYPE_FLOAT, &speed, "label='Light Speed' min=-3 max=3 step=0.1 group=Lighting");
	TwAddVarRW(bar, "Light Intensity", TW_TYPE_FLOAT, &intensity, "label='Light Intensity' min=0.0 max=2.0 step=0.1 group=Lighting");

	TwAddVarRO(bar, "Time", TW_TYPE_FLOAT, &time, "label='Time'  group=Info");
	TwAddVarRO(bar, "FPS", TW_TYPE_FLOAT, &fps, "label='FPS'  group=Info");

	TwDefine(" GLOBAL fontresizable=false ");
	TwDefine(" TW_HELP visible=false ");
	TwDefine(" ShadowVolumeOption resizable=false ");
	TwDefine(" ShadowVolumeOption size='280 230' ");
	TwDefine(" GLOBAL fontsize=3 ");
	TwDefine(" ShadowVolumeOption refresh=0.1 ");
	
	
	while (!glfwWindowShouldClose(window))
	{
		//按键监听器
		processInput(window);

		glm::mat4 projection = glm::perspective(1.0f, (float)screenWidth / (float)screenHeight, 0.2f, 50.0f);
		//glm::mat4 view = camera.getRotViewMatrix();
		glm::mat4 view = camera.GetViewMatrix();

		//加载模型
		ourModel.Draw(ourShader);
		ourShader.use();
		ourShader.setMat4("ModelViewMatrix", view);
		ourShader.setMat4("ProjMatrix", projection);
		ourShader.setMat3("NormalMatrix", glm::mat3(glm::vec3(view[0]), glm::vec3(view[1]), glm::vec3(view[2])));
		//ourModel.Draw(ourShader);

		////使球体位于屏幕下方
		//glm::mat4 modelBall = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		//glm::mat4 viewBall = glm::mat4(1.0f);
		//glm::mat4 projectionBall = glm::mat4(1.0f);
		//modelBall = glm::rotate(modelBall, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//viewBall = glm::translate(viewBall, glm::vec3(0.0f, 1.0f, 1.0f));
		//projectionBall = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		//// retrieve the matrix uniform locations
		//unsigned int modelLocBall = glGetUniformLocation(ourShader.ID, "modelBall");
		//unsigned int viewLocBall = glGetUniformLocation(ourShader.ID, "viewBall");
		//// pass them to the shaders (3 different ways)
		//glUniformMatrix4fv(modelLocBall, 1, GL_FALSE, glm::value_ptr(modelBall));
		//glUniformMatrix4fv(viewLocBall, 1, GL_FALSE, &viewBall[0][0]);
		//// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
		//ourShader.setMat4("projection", projectionBall);



		//glm::mat4 projection = glm::perspective(1.0f, (float)screenWidth / (float)screenHeight, 0.2f, 50.0f);
		////glm::mat4 view = camera.getRotViewMatrix();
		//glm::mat4 view = camera.GetViewMatrix();

		glm::mat4 mv = view * model;

		time = glfwGetTime();;

		deltaTime = time - lastFrame;
		lastFrame = time;

		fps = 1.0f / deltaTime;

		glfwPollEvents(); //获取输入
		
		// CLEAR COLOR, DEPTH E STENCIL
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// PRIMO PASSO
		// RENDERING DELLA SCENA NEL FBO
		glDepthMask(GL_TRUE);
		glDisable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_MULTISAMPLE);

		renderShader.use();
		renderShader.setMat4("ModelViewMatrix", mv);
		renderShader.setMat4("ProjMatrix", projection);
		renderShader.setMat3("NormalMatrix", glm::mat3(glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2])));
		//renderShader.setVec3("LightPosition", view*lightPosition);//意大利老铁源代码
		renderShader.setVec3("LightPosition", glm::vec3(view*lightPosition));//强制类型转换
		//renderShader.setVec4("LightPosition", view*lightPosition);//使用vec4 -> not ok
		renderShader.setFloat("LightIntensity", intensity);

		glBindFramebuffer(GL_FRAMEBUFFER, colorDepthFBO);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

		millModel.Draw(renderShader);


		planeShader.use();
		planeShader.setMat4("ModelViewMatrix",view);
		planeShader.setMat4("ProjMatrix", projection);
		planeShader.setMat3("NormalMatrix", glm::mat3(glm::vec3(view[0]), glm::vec3(view[1]), glm::vec3(view[2])));
		planeShader.setVec3("u_color", 0.6f, 0.6f, 0.6f);
		//planeShader.setVec3("LightPosition", view*lightPosition);
		planeShader.setVec3("LightPosition", glm::vec3(view*lightPosition));
		//planeShader.setVec4("LightPosition", view*lightPosition);
		planeShader.setFloat("LightIntensity", intensity);

		glBindVertexArray(VAO[0]);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		glBindVertexArray(0);

		// SECONDO PASSO
		// CREO LA SILHOUETTE DEL MODELLO DAL PUNTO DI VISTA DELLA LUCE E ALLUNGO I LATI DELLA SILHOUETTE
		// COPIO I BUFFER (COLOR E DEPTH) DEL FBO NEL DEFAULT FBO
		glBindFramebuffer(GL_READ_FRAMEBUFFER, colorDepthFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT, GL_NEAREST);

		if (!showEdge) {
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		}
		if (!showFrustum) {
			glDepthMask(GL_FALSE);
		}
		glEnable(GL_DEPTH_CLAMP);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_STENCIL_BUFFER_BIT);
		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_ALWAYS, 0, 0xff);
		glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
		glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);

		volumeShader.use();   

		volumeShader.setMat4("ModelViewMatrix", mv);
		volumeShader.setMat4("ProjMatrix", projection);
		//volumeShader.setVec3("LightPosition", view*lightPosition);
		volumeShader.setVec3("LightPosition", glm::vec3(view*lightPosition));
		//volumeShader.setVec4("LightPosition", view*lightPosition);

		millModel.Draw(volumeShader);

		glDisable(GL_DEPTH_CLAMP);
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		// TERZO PASSO
		// DISEGNO LA COMPONENTE DIFFUSA SECONDO LO STENCIL BUFFER
		glDisable(GL_DEPTH_TEST);

		glStencilFunc(GL_EQUAL, 0, 0xffff);

		compShader.use();

		glBindVertexArray(fsQuad);
		glBindTexture(GL_TEXTURE_2D, diffSpecTex);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
		glBindVertexArray(0);
		
		glDisable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);

		// SKYBOX
		glDepthFunc(GL_LEQUAL);  
		skyboxShader.use();
		//view = glm::mat4(glm::mat3(camera.getRotViewMatrix()));
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);

		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
		//

		TwDraw();

		glfwSwapBuffers(window);
		

		angle += deltaTime * speed; //speed 设置成0.1

		if (angle > glm::two_pi<float>()) {
			angle -= glm::two_pi<float>();
		}

		lightPosition = glm::vec4(3.0f * glm::vec3(cosf(angle) * distance, height, sinf(angle) * distance), 1.0f);
		
}

	TwTerminate();
	glfwTerminate();
	return 0;
}

#pragma region "User input"
//camera_mine
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
//
//void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
//{
//	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
//		glfwSetWindowShouldClose(window, GL_TRUE);
//
//	if (action == GLFW_PRESS)
//		keys[key] = true;
//	else if (action == GLFW_RELEASE)
//		keys[key] = false;
//}
//
//void mouse_callback(GLFWwindow* window, double xpos, double ypos)
//{
//	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
//		camera.updatePosition(xpos, ypos, firstMouse);
//		firstMouse = false;
//	} else {
//		camera.resetCamera();
//		firstMouse = true;
//	}
//}
//
//void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
//{
//	//camera.ProcessMouseScroll(yoffset/100);
//}

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

#pragma endregion