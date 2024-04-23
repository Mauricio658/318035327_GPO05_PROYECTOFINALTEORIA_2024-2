#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 1900, HEIGHT = 1200;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
//========================================================================
//Variables para la animación sencilla 01 -> Puertas de la Entrada
bool animacion_01 = false;
bool puertaCerrada = false;
const float MAXIMA_APERTURA = 75;
float bisagras_puertas = 0.0f;
float incremento_bisagras = 0.3f;

//========================================================================
//Variables para la animación sencilla 02 -> Puertas del Librero
bool animacion_02_izq = false;
bool animacion_02_der = false;
bool puertaIzqCerrada = false;
bool puertaDerCerrada = false;
float bisagra_izq = 0.0f;
float bisagra_der = 0.0f;
float bisagras_librero_izq = 0.0f;
float bisagras_librero_der = 0.0f;
const float MAXIMA_APERTURA_LIBRERO_IZQ = 105;
const float MAXIMA_APERTURA_LIBRERO_DER = 82;
//========================================================================
// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "318035327_ProyectoFinal_GPO13", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");

	Model Piso((char*)"Models/Pasto/Piso.obj");
	glUniform1f(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 1.0);
	Model Fachada((char*)"Models/Modelo_Casa/modelo_Casa.obj");
	Model Puerta_Izq((char*)"Models/Modelo_Casa/door_Left.obj");
	Model Puerta_Der((char*)"Models/Modelo_Casa/door_Right.obj");

	Model objeto01((char*)"Models/Objeto_01/objeto_01.obj");
	Model objeto02((char*)"Models/Objeto_02/objeto_02.obj");
	Model objeto03((char*)"Models/Objeto_03/objeto_03.obj");
	Model objeto03_Der((char*)"Models/Objeto_03/objeto_03_doorRight.obj");
	Model objeto03_Izq((char*)"Models/Objeto_03/objeto_03_doorLeft.obj");
	Model objeto04((char*)"Models/Objeto_04/objeto_04.obj");
	Model objeto05((char*)"Models/Objeto_05/objeto_05.obj");
	Model objeto06((char*)"Models/Objeto_06/objeto_06.obj");
	Model objeto07((char*)"Models/Objeto_07/objeto_07.obj");

	Model objetoExtra01((char*)"Models/objeto_Extra_01/objetoExtra_01.obj");
	Model objetoExtra02((char*)"Models/objeto_Extra_02/objetoExtra_02.obj");
	Model objetoExtra03((char*)"Models/objeto_Extra_03/objetoExtra_03.obj");
	Model objetoExtra04((char*)"Models/objeto_Extra_04/objetoExtra_04.obj");
	Model objetoExtra05((char*)"Models/objeto_Extra_05/objetoExtra_05.obj");
	Model objetoExtra06((char*)"Models/objeto_Extra_06/objetoExtra_06.obj");



	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Set texture units
	lightingShader.Use();


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);
		//Load Model
		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -0.6f, -0.2f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.5f, 0.5f, 0.5f); // Ambiental más cálido
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.8f, 0.6f, 0.4f); // Difuso más cálido
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.5f, 0.5f, 0.5f); // Especular más suave

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f); // Brillantez más suave


		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, 0.0f);
		glm::mat4 model(1);
		glm::mat4 model_Bisagra_izq(1);//Matriz Temporal para bisagras.
		glm::mat4 model_Bisagra_der(1);//Matriz Temporal para bisagras.
		glm::mat4 model_BisagraLibrero_der(1);//Matriz Temporal para bisagras.
		glm::mat4 model_BisagraLibrero_izq(1);//Matriz Temporal para bisagras.
		//glm::mat4 model_aux2(1);

		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model_Bisagra_izq = glm::mat4(1);
		model_Bisagra_der = glm::mat4(1);
		model_BisagraLibrero_der = glm::mat4(1);
		model_BisagraLibrero_izq = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//========================================================================================
		//Pasto de la pratica 80
		Piso.Draw(lightingShader);
		//========================================================================================


		//========================================================================================
		//Casa
		Fachada.Draw(lightingShader);
		//========================================================================================

		//========================================================================================
		//Puertas derecha
		model = glm::mat4(1);
		model_Bisagra_der = model = glm::translate(model, glm::vec3(6.86f, 7.83f, 4.39f));//Matriz temporal para el pivote de apertura
		model = glm::rotate(model_Bisagra_der, glm::radians(bisagras_puertas), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-3.39f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_Der.Draw(lightingShader);

		//========================================================================================
		//Puerta izquierda
		model = glm::mat4(1);
		model_Bisagra_izq = model = glm::translate(model, glm::vec3(-6.86f, 7.83f, 4.39f));//Matriz temporal para el pivote de apertura
		model = glm::rotate(model_Bisagra_izq, glm::radians(-bisagras_puertas), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(3.39f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Puerta_Izq.Draw(lightingShader);

		//========================================================================================
		//Puerta izquierda del librero
		model = glm::mat4(1);
		model_BisagraLibrero_izq = model = glm::translate(model, glm::vec3(19.10f, 4.0f, -20.83f));//Matriz temporal para el pivote de apertura
		model = glm::rotate(model_BisagraLibrero_izq, glm::radians(-bisagras_librero_izq), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(1.4f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto03_Izq.Draw(lightingShader);

		//========================================================================================
		//Puerta derecha del librero
		model = glm::mat4(1);
		model_BisagraLibrero_der = model = glm::translate(model, glm::vec3(28.31f, 4.0f, -20.83f));//Matriz temporal para el pivote de apertura
		model = glm::rotate(model_BisagraLibrero_der, glm::radians(bisagras_librero_der), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.4f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto03_Der.Draw(lightingShader);

		//========================================================================================
		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto01.Draw(lightingShader);
		objeto02.Draw(lightingShader);
		objeto03.Draw(lightingShader);
		objeto04.Draw(lightingShader);
		objeto05.Draw(lightingShader);
		objeto06.Draw(lightingShader);
		objeto07.Draw(lightingShader);

		objetoExtra01.Draw(lightingShader);
		objetoExtra02.Draw(lightingShader);
		objetoExtra03.Draw(lightingShader);
		objetoExtra04.Draw(lightingShader);
		objetoExtra05.Draw(lightingShader);
		objetoExtra06.Draw(lightingShader);

		//glBindVertexArray(0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	//===================================================================
	//Animación para realizar la apertura de la entrada
	if (puertaCerrada) {
		if (bisagras_puertas < MAXIMA_APERTURA) { //Si la apertura es menor al máximo
			bisagras_puertas += incremento_bisagras; //incrementamos las bisagras
		}
		else {
			// La animación ha terminado, restablecer la variable
			animacion_01 = false;
		}
	}
	else {
		if (bisagras_puertas >= 0) {
			bisagras_puertas -= incremento_bisagras;
		}
		else {
			// La animación ha terminado, restablecer la variable
			animacion_01 = false;
		}
	}
	//===================================================================
	//Animación para realizar la apertura de la puerta izquierda del librero
	if (puertaIzqCerrada) {
		if (bisagras_librero_izq < MAXIMA_APERTURA_LIBRERO_IZQ) { //Si la apertura es menor al máximo
			bisagras_librero_izq += incremento_bisagras; //incrementamos las bisagras
		}
		else {
			animacion_02_izq = false;
		}

	}
	else {
		if (bisagras_librero_izq >= 0) {
			bisagras_librero_izq -= incremento_bisagras;
		}
		else {
			animacion_02_izq = false;
		}
	}

	//===================================================================
	//Animación para realizar la apertura de la puerta derecha del librero
	if (puertaDerCerrada) {
		if (bisagras_librero_der < MAXIMA_APERTURA_LIBRERO_DER) { //Si la apertura es menor al máximo
			bisagras_librero_der += incremento_bisagras; //incrementamos las bisagras
		}
		else {
			animacion_02_der = false;
		}
	}
	else {
		if (bisagras_librero_der >= 0) {
			bisagras_librero_der -= incremento_bisagras;
		}
		else {
			animacion_02_der = false;
		}
	}
	//===================================================================
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}
	//==================================================================================================
	//Tecla Espacio para activar la animación 1 -> Apertura de Puertas Principal
	if (!animacion_01) {
		if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		{
			// Si la puerta está cerrada, abrir
			if (puertaCerrada) {
				puertaCerrada = false; // La puerta se cierra
			}
			else { // Si la puertaa está abierta, cerrar
				puertaCerrada = true; // La puerta está cerrada nuevamente
			}
			animacion_01 = true;
		}
	}
	//==================================================================================================
	//Tecla Z para activar la animación 2 -> Apertura de Puerta izquierda del librero
	if (!animacion_02_izq) {
		if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
			if (puertaIzqCerrada) {
				puertaIzqCerrada = false;
			}
			else {
				puertaIzqCerrada = true;
			}
			animacion_02_izq = true;
		}
	}
	//==================================================================================================
	//Tecla X para activar la animación 2 -> Apertura de Puerta derecha del librero
	if (!animacion_02_der) {
		if (key == GLFW_KEY_X && action == GLFW_PRESS) {
			if (puertaDerCerrada) {
				puertaDerCerrada = false;
			}
			else {
				puertaDerCerrada = true;
			}
			animacion_02_der = true;
		}
	}
	//==================================================================================================
}



void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}