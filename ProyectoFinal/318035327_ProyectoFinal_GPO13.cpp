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
Camera  camera(glm::vec3(0.0f, 6.0f, 20.0f));
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
float bisagras_librero_izq = 0.0f;
float bisagras_librero_der = 0.0f;
const float MAXIMA_APERTURA_LIBRERO_IZQ = 105;
const float MAXIMA_APERTURA_LIBRERO_DER = 82;

//========================================================================
//Variables para la aimación sencilla 03 -> Libro 01
bool animacion_02_libro01 = false;
bool libro_trasladado1 = false;
float rotacion_ejey = 0.0f;
float rotacion_ejez = 0.0f;
const float ROTACION_MAXIMA_Y = 90;
const float INCLINACION_MAXIMA = 15;
const float velocidad_rotacion = 0.5;
const float velocidad_traslacion = 0.07;
const float TRASLACION_MAXIMA = 2.0;
float traslacion_libro01 = 0.0f;

//========================================================================================
//Variables para la aimación sencilla 03 -> Libro 02
bool animacion_02_libro02 = false;
bool libro_trasladado2 = false;
float rotacion_ejey1 = 0.0f;
float rotacion_ejez1 = 0.0f;
float traslacion_libro02 = 0.0f;

//========================================================================================
//Variables para la aimación sencilla 03 -> Libro 03
bool animacion_02_libro03 = false;
bool libro_trasladado3 = false;
float rotacion_ejey2 = 0.0f;
float rotacion_ejez2 = 0.0f;
float traslacion_libro03 = 0.0f;

//========================================================================================
//Variables para la animación sencilla 04 -> Puertas del escritorio
bool animacion_04 = false;
bool puerta_escritorio = false;
float bisagra_escritorio = 0.0f;
float bisagra_caja = 0.0f;
const float MAXIMA_APERTURA_ESCRITORIO = 90;
const float MAXIMA_APERTURA_CAJA = 82;
//========================================================================
//Variables para la animación compleja 01 -> Movimiento de la flama
bool Compleja_01 = false;
float tiempo = 0.0f;

//========================================================================
//Variables para la animación compleja 02 -> Movimiento de la chimenea
bool Compleja_02 = false;
float tiempo2 = 0.0f;

//========================================================================
//Variables para la animación compleja 03 -> Movimiento de la estufa
bool Compleja_03 = false;
float tiempo3 = 0.0f;
float transparenciaHumo = 0.0f;
const float MAXIMA_TRANSPARENCIA = 0.05f;
//========================================================================
//Variables para la animación sencilla 05  -> Movimiento de la protogema
bool animacionSencilla_05 = false;
float proto = 0.0f;

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


	//Declaración de Shaders a Ocupar
	Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
	Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
	Shader animacionCompleja_01("Shaders/animacionCompleja_01.vs", "Shaders/animacionCompleja_01.frag");
	Shader animacionCompleja_02("Shaders/animacionCompleja_02.vs", "Shaders/animacionCompleja_02.frag");
	Shader animacionCompleja_03("Shaders/animacionCompleja_03.vs", "Shaders/animacionCompleja_03.frag");
	Shader animacionCompleja_04("Shaders/animacionCompleja_04.vs", "Shaders/animacionCompleja_04.frag");
	Shader animacionCompleja_05("Shaders/animacionCompleja_05.vs", "Shaders/animacionCompleja_05.frag");
	Shader animacionSencilla_03("Shaders/animacionSencilla_03.vs", "Shaders/animacionSencilla_03.frag");

	//Declaracion de Modelos a usar
	Model Piso((char*)"Models/Pasto/Piso.obj");
	Model Fachada((char*)"Models/Modelo_Casa/modelo_Casa.obj");
	Model Puerta_Izq((char*)"Models/Modelo_Casa/door_Left.obj");
	Model Puerta_Der((char*)"Models/Modelo_Casa/door_Right.obj");

	Model objeto01((char*)"Models/Modelo_Objetos/Objeto_01/objeto_01.obj");
	Model objeto02((char*)"Models/Modelo_Objetos/Objeto_02/objeto_02.obj");

	Model objeto03((char*)"Models/Modelo_Objetos/Objeto_03/objeto_03.obj");
	Model objeto03_Der((char*)"Models/Modelo_Objetos/Objeto_03/objeto_03_doorRight.obj");
	Model objeto03_Izq((char*)"Models/Modelo_Objetos/Objeto_03/objeto_03_doorLeft.obj");
	Model objeto03_Libro01((char*)"Models/Modelo_Objetos/Objeto_03/objeto_03_libro01.obj");
	Model objeto03_Libro02((char*)"Models/Modelo_Objetos/Objeto_03/objeto_03_libro02.obj");
	Model objeto03_Libro03((char*)"Models/Modelo_Objetos/Objeto_03/objeto_03_libro03.obj");

	Model objeto04((char*)"Models/Modelo_Objetos/Objeto_04/objeto_04.obj");
	Model objeto04_protogema((char*)"Models/Modelo_Objetos/Objeto_04/objeto_04_protogema.obj");
	Model objeto04_flama((char*)"Models/Modelo_Objetos/Objeto_04/objeto_04_flama.obj");
	Model objeto04_Puerta((char*)"Models/Modelo_Objetos/Objeto_04/objeto_04_door.obj");
	Model objeto04_Puerta_Caja((char*)"Models/Modelo_Objetos/Objeto_04/objeto_04_doorCaja.obj");

	Model objeto05((char*)"Models/Modelo_Objetos/Objeto_05/objeto_05.obj");
	Model objeto06((char*)"Models/Modelo_Objetos/Objeto_06/objeto_06.obj");

	Model objeto07((char*)"Models/Modelo_Objetos/Objeto_07/objeto_07.obj");
	Model objeto07_flama((char*)"Models/Modelo_Objetos/Objeto_07/objeto_07_flama.obj");
	Model objeto07_aceite((char*)"Models/Modelo_Objetos/Objeto_07/objeto_07_aceite.obj");
	Model objeto07_humo((char*)"Models/Modelo_Objetos/Objeto_07/objeto_07_humo.obj");

	Model objetoExtra01((char*)"Models/Modelos_Objetos_Extras/objeto_Extra_01/objetoExtra_01.obj");
	Model objetoExtra01_flama((char*)"Models/Modelos_Objetos_Extras/objeto_Extra_01/objetoExtra_01_flama.obj");
	Model objetoExtra02((char*)"Models/Modelos_Objetos_Extras/objeto_Extra_02/objetoExtra_02.obj");
	Model objetoExtra03((char*)"Models/Modelos_Objetos_Extras/objeto_Extra_03/objetoExtra_03.obj");
	Model objetoExtra04((char*)"Models/Modelos_Objetos_Extras/objeto_Extra_04/objetoExtra_04.obj");
	Model objetoExtra05((char*)"Models/Modelos_Objetos_Extras/objeto_Extra_05/objetoExtra_05.obj");
	Model objetoExtra06((char*)"Models/Modelos_Objetos_Extras/objeto_Extra_06/objetoExtra_06.obj");



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
		//======================================================================================
		//Matrices Auxiliares
		glm::mat4 model(1);
		glm::mat4 model_Bisagra_izq(1);//Matriz Temporal para bisagras.
		glm::mat4 model_Bisagra_der(1);//Matriz Temporal para bisagras.
		glm::mat4 model_BisagraLibrero_der(1);//Matriz Temporal para bisagras.
		glm::mat4 model_BisagraLibrero_izq(1);//Matriz Temporal para bisagras.
		glm::mat4 model1_Libro01(1);//Matriz Temporal para libro 01
		glm::mat4 model2_Libro01(1);//Matriz Temporal para libro 01		
		glm::mat4 model1_Libro02(1);//Matriz Temporal para libro 02
		glm::mat4 model2_Libro02(1);//Matriz Temporal para libro 02
		glm::mat4 model1_Libro03(1);//Matriz Temporal para libro 03
		glm::mat4 model2_Libro03(1);//Matriz Temporal para libro 03

		glm::mat4 model_Bisagra_escritorio(1);//Matriz Temporal para bisagras.
		glm::mat4 model_Bisagra_caja(1);//Matriz Temporal para bisagras.
		//glm::mat4 model_aux2(1);

		//Carga de modelo 
		view = camera.GetViewMatrix();
		model = glm::mat4(1);
		model_Bisagra_izq = glm::mat4(1);
		model_Bisagra_der = glm::mat4(1);
		model_BisagraLibrero_der = glm::mat4(1);
		model_BisagraLibrero_izq = glm::mat4(1);
		model1_Libro01 = glm::mat4(1);
		model2_Libro01 = glm::mat4(1);
		model1_Libro02 = glm::mat4(1);
		model2_Libro02 = glm::mat4(1);
		model1_Libro03 = glm::mat4(1);
		model2_Libro03 = glm::mat4(1);
		model_Bisagra_escritorio = glm::mat4(1);
		model_Bisagra_caja = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//========================================================================================
		//Pasto de la pratica 80
		Piso.Draw(lightingShader);

		//========================================================================================
		//Casa
		Fachada.Draw(lightingShader);

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
		//Libro 01 del librero
		model = glm::mat4(1);
		model1_Libro01 = model = glm::translate(model, glm::vec3(24.86f, 10.0f, -21.60f)); //Posición en el librero
		model2_Libro01 = model = glm::translate(model1_Libro01, glm::vec3(0.0f, 0.0f, traslacion_libro01)); //Posición en el librero
		model2_Libro01 = model = glm::rotate(model2_Libro01, glm::radians(-rotacion_ejey), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model2_Libro01, glm::radians(rotacion_ejez), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto03_Libro01.Draw(lightingShader);

		//========================================================================================
		//Libro 02 del librero
		model = glm::mat4(1);
		model1_Libro02 = model = glm::translate(model, glm::vec3(23.68f, 7.95f, -21.60f)); //Posición en el librero
		model2_Libro02 = model = glm::translate(model1_Libro02, glm::vec3(0.0f, 0.0f, traslacion_libro02)); //Posición en el librero
		model2_Libro02 = model = glm::rotate(model2_Libro02, glm::radians(-rotacion_ejey1), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model2_Libro02, glm::radians(rotacion_ejez1), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto03_Libro02.Draw(lightingShader);

		//========================================================================================
		//Libro 03 del librero
		model = glm::mat4(1);
		model1_Libro03 = model = glm::translate(model, glm::vec3(24.56f, 6.03f, -21.60f)); //Posición en el librero
		model2_Libro03 = model = glm::translate(model1_Libro03, glm::vec3(0.0f, 0.0f, traslacion_libro03)); //Posición en el librero
		model2_Libro03 = model = glm::rotate(model2_Libro03, glm::radians(-rotacion_ejey2), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model2_Libro03, glm::radians(rotacion_ejez2), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto03_Libro03.Draw(lightingShader);

		//========================================================================================
		//Puerta del Librero
		model = glm::mat4(1);
		model_Bisagra_escritorio = model = glm::translate(model, glm::vec3(26.65f, 3.70f, -13.0f));//Matriz temporal para el pivote de apertura
		model = glm::rotate(model_Bisagra_escritorio, glm::radians(-bisagra_escritorio), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-1.16f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto04_Puerta.Draw(lightingShader);

		//========================================================================================
		//Puerta de la Caja Fuerte
		model = glm::mat4(1);
		model_Bisagra_caja = model = glm::translate(model, glm::vec3(24.49f, 3.54f, -11.88f));//Matriz temporal para el pivote de apertura
		model = glm::rotate(model_Bisagra_caja, glm::radians(bisagra_caja), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.89f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto04_Puerta_Caja.Draw(lightingShader);

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
		glBindVertexArray(0);

		//========================================================================================
		animacionSencilla_03.Use();
		modelLoc = glGetUniformLocation(animacionSencilla_03.Program, "model");
		viewLoc = glGetUniformLocation(animacionSencilla_03.Program, "view");
		projLoc = glGetUniformLocation(animacionSencilla_03.Program, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		model = glm::mat4(1);
		glUniform1f(glGetUniformLocation(animacionSencilla_03.Program, "time"), proto);
		model = glm::translate(model, glm::vec3(25.45f, 2.86f, -11.30f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto04_protogema.Draw(animacionSencilla_03);

		//========================================================================================
		//Animación Compleja

		animacionCompleja_01.Use();
		//tiempo = glfwGetTime();//normalizar la velocidad
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(animacionCompleja_01.Program, "model");
		viewLoc = glGetUniformLocation(animacionCompleja_01.Program, "view");
		projLoc = glGetUniformLocation(animacionCompleja_01.Program, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(animacionCompleja_01.Program, "activaTransparencia"), 0.0);
		//Vela individual
		glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniform4f(glGetUniformLocation(animacionCompleja_01.Program, "colorAlpha"), 1.0f, 1.0f, 0.7f, 0.60f);
		model = glm::mat4(1);
		glUniform1f(glGetUniformLocation(animacionCompleja_01.Program, "time"), tiempo);
		model = glm::translate(model, glm::vec3(18.96f, 6.05f, -10.89f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto04_flama.Draw(animacionCompleja_01);

		//========================================================================================
		//Candelero
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(23.91f, 7.24f, -10.86f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto04_flama.Draw(animacionCompleja_01);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(24.87f, 7.24f, -10.86f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto04_flama.Draw(animacionCompleja_01);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(24.39f, 7.46f, -10.86f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto04_flama.Draw(animacionCompleja_01);

		//========================================================================================
		//Candelero mesa
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(1.75f, 6.25f, -7.93f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto04_flama.Draw(animacionCompleja_01);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.10f, 6.4f, -7.93f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto04_flama.Draw(animacionCompleja_01);

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(2.44f, 6.25f, -7.93f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto04_flama.Draw(animacionCompleja_01);
		glBindVertexArray(0);

		//========================================================================================
		//flama de la estufa
		animacionCompleja_03.Use();
		modelLoc = glGetUniformLocation(animacionCompleja_03.Program, "model");
		viewLoc = glGetUniformLocation(animacionCompleja_03.Program, "view");
		projLoc = glGetUniformLocation(animacionCompleja_03.Program, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(animacionCompleja_03.Program, "activaTransparencia"), 0.0);
		glUniform4f(glGetUniformLocation(animacionCompleja_03.Program, "colorAlpha"), 1.0f, 1.0f, 0.0f, 0.60f);
		model = glm::mat4(1);
		glUniform1f(glGetUniformLocation(animacionCompleja_03.Program, "time"), tiempo3);
		model = glm::translate(model, glm::vec3(24.99f, 2.19f, -1.89f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto07_flama.Draw(animacionCompleja_03);
		glBindVertexArray(0);



		//========================================================================================
		//flama de la chimenea
		animacionCompleja_02.Use();
		modelLoc = glGetUniformLocation(animacionCompleja_02.Program, "model");
		viewLoc = glGetUniformLocation(animacionCompleja_02.Program, "view");
		projLoc = glGetUniformLocation(animacionCompleja_02.Program, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(animacionCompleja_02.Program, "activaTransparencia"), 0.0);
		glUniform4f(glGetUniformLocation(animacionCompleja_02.Program, "colorAlpha"), 1.0f, 1.0f, 0.0f, 0.60f);
		model = glm::mat4(1);
		glUniform1f(glGetUniformLocation(animacionCompleja_02.Program, "time"), tiempo2);

		model = glm::translate(model, glm::vec3(-30.74f, 2.91f, -10.75f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objetoExtra01_flama.Draw(animacionCompleja_02);
		glBindVertexArray(0);


		//========================================================================================
		//Aceite de la estufa
		animacionCompleja_04.Use();
		modelLoc = glGetUniformLocation(animacionCompleja_04.Program, "model");
		viewLoc = glGetUniformLocation(animacionCompleja_04.Program, "view");
		projLoc = glGetUniformLocation(animacionCompleja_04.Program, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(animacionCompleja_04.Program, "activaTransparencia"), 0.0);
		glUniform4f(glGetUniformLocation(animacionCompleja_04.Program, "colorAlpha"), 1.0f, 1.0f, 0.0f, 0.10f);
		model = glm::mat4(1);
		glUniform1f(glGetUniformLocation(animacionCompleja_04.Program, "time"), tiempo3);

		model = glm::translate(model, glm::vec3(25.15f, 3.58f, -1.88f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		objeto07_aceite.Draw(animacionCompleja_04);
		glBindVertexArray(0);

		//========================================================================================
		//Humo de la estufa
		animacionCompleja_05.Use();
		modelLoc = glGetUniformLocation(animacionCompleja_05.Program, "model");
		viewLoc = glGetUniformLocation(animacionCompleja_05.Program, "view");
		projLoc = glGetUniformLocation(animacionCompleja_05.Program, "projection");
		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1f(glGetUniformLocation(animacionCompleja_05.Program, "activaTransparencia"), 0.0);
		glUniform4f(glGetUniformLocation(animacionCompleja_05.Program, "colorAlpha"), 1.0f, 1.0f, 1.0f, transparenciaHumo);
		model = glm::mat4(1);
		glUniform1f(glGetUniformLocation(animacionCompleja_05.Program, "time"), tiempo3);

		model = glm::translate(model, glm::vec3(25.19f, 4.72f, -2.01f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//if (Compleja_03) {
		objeto07_humo.Draw(animacionCompleja_05);
		//}


		glBindVertexArray(0);
		glDisable(GL_BLEND);  //Desactiva el canal alfa
		//========================================================================================
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
	//Animación para realizar la el movimiento del libro 01 del librero
	if (libro_trasladado1) {
		if (traslacion_libro01 < TRASLACION_MAXIMA) {
			traslacion_libro01 += velocidad_traslacion;
		}
		else {
			if (rotacion_ejey < ROTACION_MAXIMA_Y) {
				rotacion_ejey += velocidad_rotacion;
			}
			else {
				if (rotacion_ejez < INCLINACION_MAXIMA) {
					rotacion_ejez += velocidad_rotacion;
				}
				else {
					animacion_02_libro01 = false;
				}
			}
		}
	}
	else {
		if (rotacion_ejez >= 0) {
			rotacion_ejez -= velocidad_rotacion;
		}
		else {
			if (rotacion_ejey >= 0) {
				rotacion_ejey -= velocidad_rotacion;
			}
			else {
				if (traslacion_libro01 >= 0) {
					traslacion_libro01 -= velocidad_traslacion;
				}
				else {
					animacion_02_libro01 = false;
				}
			}
		}
	}
	//===================================================================
	//Animación para realizar la el movimiento del libro 02 del librero
	if (libro_trasladado2) {
		if (traslacion_libro02 < TRASLACION_MAXIMA) {
			traslacion_libro02 += velocidad_traslacion;
		}
		else {
			if (rotacion_ejey1 < ROTACION_MAXIMA_Y) {
				rotacion_ejey1 += velocidad_rotacion;
			}
			else {
				if (rotacion_ejez1 < INCLINACION_MAXIMA) {
					rotacion_ejez1 += velocidad_rotacion;
				}
				else {
					animacion_02_libro02 = false;
				}
			}
		}
	}
	else {
		if (rotacion_ejez1 >= 0) {
			rotacion_ejez1 -= velocidad_rotacion;
		}
		else {
			if (rotacion_ejey1 >= 0) {
				rotacion_ejey1 -= velocidad_rotacion;
			}
			else {
				if (traslacion_libro02 >= 0) {
					traslacion_libro02 -= velocidad_traslacion;
				}
				else {
					animacion_02_libro02 = false;
				}
			}
		}
	}

	//===================================================================
	//Animación para realizar la el movimiento del libro 02 del librero
	if (libro_trasladado3) {
		if (traslacion_libro03 < TRASLACION_MAXIMA) {
			traslacion_libro03 += velocidad_traslacion;
		}
		else {
			if (rotacion_ejey2 < ROTACION_MAXIMA_Y) {
				rotacion_ejey2 += velocidad_rotacion;
			}
			else {
				if (rotacion_ejez2 < INCLINACION_MAXIMA) {
					rotacion_ejez2 += velocidad_rotacion;
				}
				else {
					animacion_02_libro03 = false;
				}
			}
		}
	}
	else {
		if (rotacion_ejez2 >= 0) {
			rotacion_ejez2 -= velocidad_rotacion;
		}
		else {
			if (rotacion_ejey2 >= 0) {
				rotacion_ejey2 -= velocidad_rotacion;
			}
			else {
				if (traslacion_libro03 >= 0) {
					traslacion_libro03 -= velocidad_traslacion;
				}
				else {
					animacion_02_libro03 = false;
				}
			}
		}
	}

	//===================================================================
	//Animación para realizar el movimiento del las puerta del librero y caja fuerte
	if (puerta_escritorio) {
		if (bisagra_escritorio < MAXIMA_APERTURA_ESCRITORIO) {
			bisagra_escritorio += velocidad_rotacion;
		}
		else {
			if (bisagra_caja < MAXIMA_APERTURA_CAJA) {
				bisagra_caja += velocidad_rotacion;
			}
			else {
				animacion_04 = false;
			}
		}
	}
	else {
		if (bisagra_caja >= 0) {
			bisagra_caja -= velocidad_rotacion;
		}
		else {
			if (bisagra_escritorio >= 0) {
				bisagra_escritorio -= velocidad_rotacion;
			}
			else {
				animacion_04 = false;
			}
		}
	}

	//===================================================================
	//Animación Compleja 01 -> Movimento de una vela
	if (Compleja_01) {
		tiempo = glfwGetTime();
	}
	else {
		tiempo = 0.0f;
	}
	//===================================================================
	//Animación Compleja 02 -> Flama de la Chimenea
	if (Compleja_02) {
		tiempo2 = glfwGetTime();
	}
	else {
		tiempo2 = 0.0f;
	}
	//===================================================================
	//Animación Compleja 03 -> Flama de la Estufa, aceite y humo
	if (Compleja_03) {
		tiempo3 = glfwGetTime();
		if (transparenciaHumo <= MAXIMA_TRANSPARENCIA) {
			transparenciaHumo += 0.001f;
		}
	}
	else {
		if (transparenciaHumo >= 0.0f) {
			transparenciaHumo -= 0.001f;
		}
		tiempo3 = 0.0f;
	}

	//===================================================================
	//Animación Sencilla 3 -> Movimento de la protogema
	if (animacionSencilla_05) {
		proto = glfwGetTime();
	}
	else {
		proto = 0.0f;
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
	 // Tecla 1 para activar la animación del libro
	if (!animacion_02_libro01)
	{
		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
		{
			if (libro_trasladado1) {
				libro_trasladado1 = false;
			}
			else {
				libro_trasladado1 = true;
			}
			animacion_02_libro01 = true;
		}
	}
	//==================================================================================================
	 // Tecla 2 para activar la animación del libro
	if (!animacion_02_libro02)
	{
		if (key == GLFW_KEY_2 && action == GLFW_PRESS)
		{
			if (libro_trasladado2) {
				libro_trasladado2 = false;
			}
			else {
				libro_trasladado2 = true;
			}
			animacion_02_libro02 = true;
		}
	}
	//==================================================================================================
	// Tecla 3 para activar la animación del libro
	if (!animacion_02_libro03)
	{
		if (key == GLFW_KEY_3 && action == GLFW_PRESS)
		{
			if (libro_trasladado3) {
				libro_trasladado3 = false;
			}
			else {
				libro_trasladado3 = true;
			}
			animacion_02_libro03 = true;
		}
	}
	//==================================================================================================
	// Tecla C para activar la animación del las puertas del escritorio
	if (!animacion_04)
	{
		if (key == GLFW_KEY_C && action == GLFW_PRESS)
		{
			if (puerta_escritorio) {
				puerta_escritorio = false;
			}
			else {
				puerta_escritorio = true;
			}
			animacion_04 = true;
		}
	}
	//==================================================================================================
	//Tecla N para activar la animación Compleja 01
	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		Compleja_01 = !Compleja_01;
	}

	//==================================================================================================
	//Tecla V para activar la animación Compleja 01
	if (key == GLFW_KEY_V && action == GLFW_PRESS) {
		Compleja_02 = !Compleja_02;
	}
	//==================================================================================================
	//Tecla M para activar la animación Compleja 01
	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		Compleja_03 = !Compleja_03;
	}

	//==================================================================================================
	//Tecla B para activar la animación Sencilla 05
	if (key == GLFW_KEY_B && action == GLFW_PRESS) {
		animacionSencilla_05 = !animacionSencilla_05;
	}
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