#include "SP2.h"
#include "GL\glew.h"
#include <sstream>

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Vertex.h"

#include "LoadTGA.h"


SP2::SP2()
{
}

SP2::~SP2()
{
}

void SP2::Init()
{

	data.ReadTextFile( "OBJ-Pos/Martpos.txt" );
	shelve.ReadTextFile( "OBJ-Pos/Shelves.txt" );
	cashier.ReadTextFile( "OBJ-Pos/Cashierpos.txt" );
	fridge.ReadTextFile( "OBJ-Pos/Fridgepos.txt" );
	character.ReadTextFile( "OBJ-Pos/Characterpos.txt" );
	cereal.ReadTextFile( "OBJ-Pos/Cerealpos.txt" );
	watercan.ReadTextFile( "OBJ-Pos/WaterCanpos.txt" );
	mtdew.ReadTextFile( "OBJ-Pos/MountainDew.txt" );
	coke.ReadTextFile( "OBJ-Pos/Cokepos.txt" );
	pepsi.ReadTextFile( "OBJ-Pos/Pepsipos.txt" );
	chocolate.ReadTextFile( "OBJ-Pos/Chocolatepos.txt" );
	chips.ReadTextFile( "OBJ-Pos/Chipspos.txt" );
	canfood.ReadTextFile( "OBJ-Pos/CanFoodpos.txt" );
	shopper.ReadTextFilePath( "OBJ-Pos/CharacterPath.txt");
	pizza.ReadTextFile("OBJ-Pos/Pizzapos.txt");
	icecream.ReadTextFile("OBJ-Pos/Icecreampos.txt");
	mcncheese.ReadTextFile("OBJ-Pos/Mcncheesepos.txt");
	sardines.ReadTextFile("OBJ-Pos/Sardinespos.txt");
	ayamcan.ReadTextFile("OBJ-Pos/Ayamcanpos.txt");
	chickenstock.ReadTextFile("OBJ-Pos/Chickenstockpos.txt");
	toblerone.ReadTextFile("OBJ-Pos/Tobleronepos.txt");
	milo.ReadTextFile("OBJ-Pos/Milopos.txt");
	vegcan.ReadTextFile("OBJ-Pos/Vegcanpos.txt");
	sardcan.ReadTextFile("OBJ-Pos/Sardcanpos.txt");
	cactus.ReadTextFile("OBJ-Pos/Cactuspos.txt");
	reditos.ReadTextFile("OBJ-Pos/Reditospos.txt");
	dewitos.ReadTextFile("OBJ-Pos/Dewitospos.txt");
	diabetos.ReadTextFile("OBJ-Pos/Diabetospos.txt");
	moaties.ReadTextFile("OBJ-Pos/Moatiespos.txt");

	// Set background color to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);
	//Enable back face culling
	glEnable(GL_CULL_FACE);
	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);
	
	//Load vertex and fragment shaders
	//m_programID = LoadShaders( "Shader//TransformVertexShader.vertexshader", "Shader//SimpleFragmentShader.fragmentshader" );
	//m_programID = LoadShaders( "Shader//Shading.vertexshader", "Shader//Shading.fragmentshader" );
	/*m_programID = LoadShaders( "Shader//Shading.vertexshader", "Shader//LightSource.fragmentshader" );*/
		//m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//Texture.fragmentshader" );
	m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//Blending.fragmentshader" );
	m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//Text.fragmentshader" );
	// Use our shader
	//glUseProgram(m_programID);
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	//variable to rotate geometry
	rotateAngle = 0;

	//Initialize camera settings
	camera.Init(Vector3(0, 5, 100), Vector3(0, 0, 0), Vector3(0, 1, 0));
	
	//Initialize all meshes to NULL
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);


	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 2000.f);
	projectionStack.LoadMatrix(projection);

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID,"lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID,"lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID,"lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID,"lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID,"lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID,"lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID,"lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID,"lights[1].exponent");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID,"numLights");
	
		// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");

	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");



	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_SPOT;
	lights[0].position.Set(0, 20, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_POINT;
	lights[1].position.Set(20, 20, 0);
	lights[1].color.Set(1, 1, 1);
	lights[1].power = 1;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_NUMLIGHTS], 2);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	//Light ball
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 10, 10, 1);
	meshList[GEO_LIGHTBALL2] = MeshBuilder::GenerateSphere("lightball2", Color(1, 1, 1), 10, 10, 1);

	//meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	//meshList[GEO_QUAD]->textureID = LoadTGA("Image//color2.tga");

	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//()Town_bk.tga");

	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//()Town_ft.tga");

	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//()Town_dn.tga");

	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//()Town2_Up.tga");

	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//()Town_rt.tga");

	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("front", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//()Town_lf.tga");

	meshList[GEO_MODEL_DOORMAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//doorman.obj");
	meshList[GEO_MODEL_DOORMAN]->textureID = LoadTGA("Image//doorman.tga");

	meshList[GEO_MODEL_MART] = MeshBuilder::GenerateOBJ("model1", "OBJ//Mart.obj");
	meshList[GEO_MODEL_MART]->textureID = LoadTGA("Image//supermart.tga");

	meshList[GEO_MODEL_DOOR] = MeshBuilder::GenerateOBJ("model1", "OBJ//Door.obj");
	meshList[GEO_MODEL_DOOR]->textureID = LoadTGA("Image//Door.tga");

	meshList[GEO_MODEL_CASHIER] = MeshBuilder::GenerateOBJ("model1", "OBJ//cashier table.obj");
	meshList[GEO_MODEL_CASHIER]->textureID = LoadTGA("Image//cashier table.tga");

	meshList[GEO_MODEL_CHAR1] = MeshBuilder::GenerateOBJ("model1", "OBJ//char1.obj");
	meshList[GEO_MODEL_CHAR1]->textureID = LoadTGA("Image//Door.tga");

	meshList[GEO_MODEL_FRIDGE] = MeshBuilder::GenerateOBJ("model1", "OBJ//fridge.obj");
	meshList[GEO_MODEL_FRIDGE]->textureID = LoadTGA("Image//fridge.tga");

	meshList[GEO_MODEL_SHELF] = MeshBuilder::GenerateOBJ("model1", "OBJ//shelf.obj");
	meshList[GEO_MODEL_SHELF]->textureID = LoadTGA("Image//Shelf_Texture2.tga");

	meshList[GEO_MODEL_CEREAL_1] = MeshBuilder::GenerateOBJ("model1", "OBJ//cerealbox1.obj");
	meshList[GEO_MODEL_CEREAL_1]->textureID = LoadTGA("Image//cereal box 1.tga");

	meshList[GEO_MODEL_CEREAL_2] = MeshBuilder::GenerateOBJ("model1", "OBJ//cerealbox2.obj");
	meshList[GEO_MODEL_CEREAL_2]->textureID = LoadTGA("Image//cereal box 2.tga");

	meshList[GEO_MODEL_CEREAL_3] = MeshBuilder::GenerateOBJ("model1", "OBJ//cerealbox3.obj");
	meshList[GEO_MODEL_CEREAL_3]->textureID = LoadTGA("Image//cereal box 3.tga");

	meshList[GEO_MODEL_WATERCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//water.obj");
	meshList[GEO_MODEL_WATERCAN]->textureID = LoadTGA("Image//water.tga");

	meshList[GEO_MODEL_MTDEW] = MeshBuilder::GenerateOBJ("model1", "OBJ//mountainDew.obj");
	meshList[GEO_MODEL_MTDEW]->textureID = LoadTGA("Image//Mountain Dew Texture.tga");

	meshList[GEO_MODEL_COKE] = MeshBuilder::GenerateOBJ("model1", "OBJ//coke.obj");
	meshList[GEO_MODEL_COKE]->textureID = LoadTGA("Image//Coke.tga");

	meshList[GEO_MODEL_PEPSI] = MeshBuilder::GenerateOBJ("model1", "OBJ//pepsi.obj");
	meshList[GEO_MODEL_PEPSI]->textureID = LoadTGA("Image//Pepsi.tga");

	meshList[GEO_MODEL_HERSHEY] = MeshBuilder::GenerateOBJ("model1", "OBJ//hershey.obj");
	meshList[GEO_MODEL_HERSHEY]->textureID = LoadTGA("Image//hershey.tga");

	meshList[GEO_MODEL_ROCHER] = MeshBuilder::GenerateOBJ("model1", "OBJ//FerreroRocher.obj");
	meshList[GEO_MODEL_ROCHER]->textureID = LoadTGA("Image//rocher.tga");

	meshList[GEO_MODEL_BBCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//bbCan.obj");
	meshList[GEO_MODEL_BBCAN]->textureID = LoadTGA("Image//bbCan.tga");

	meshList[GEO_MODEL_PUMPKINCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//pumpkinCan.obj");
	meshList[GEO_MODEL_PUMPKINCAN]->textureID = LoadTGA("Image//pumpkinCan.tga");

	meshList[GEO_MODEL_TOMATOSOUPCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//tomatoSoupCan.obj");
	meshList[GEO_MODEL_TOMATOSOUPCAN]->textureID = LoadTGA("Image//tomatoSoupCan.tga");

	meshList[GEO_MODEL_PEASNCARROTCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//peasNcarrot.obj");
	meshList[GEO_MODEL_PEASNCARROTCAN]->textureID = LoadTGA("Image//peasNcarrot.tga");

	meshList[GEO_MODEL_LAYSCHIPS] = MeshBuilder::GenerateOBJ("model1", "OBJ//LaysChips.obj");
	meshList[GEO_MODEL_LAYSCHIPS]->textureID = LoadTGA("Image//LaysChips.tga");

	meshList[GEO_MODEL_FLOOR] = MeshBuilder::GenerateOBJ("model1", "OBJ//Floor.obj");
	meshList[GEO_MODEL_FLOOR]->textureID = LoadTGA("Image//Floor.tga");

	meshList[GEO_MODEL_PIZZA] = MeshBuilder::GenerateOBJ("model1", "OBJ//pizza.obj");
	meshList[GEO_MODEL_PIZZA]->textureID = LoadTGA("Image//frozen_pizza.tga");
	
	meshList[GEO_MODEL_ICECREAM] = MeshBuilder::GenerateOBJ("model1", "OBJ//icecream.obj");
	meshList[GEO_MODEL_ICECREAM]->textureID = LoadTGA("Image//ice_cream_box.tga");

	meshList[GEO_MODEL_MCNCHEESE] = MeshBuilder::GenerateOBJ("model1", "OBJ//mcncheese.obj");
	meshList[GEO_MODEL_MCNCHEESE]->textureID = LoadTGA("Image//Box_Tex.tga");

	meshList[GEO_MODEL_SARDINES] = MeshBuilder::GenerateOBJ("model1", "OBJ//sardines.obj");
	meshList[GEO_MODEL_SARDINES]->textureID = LoadTGA("Image//Can_Tex.tga");

	meshList[GEO_MODEL_AYAMCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//ayamcan.obj");
	meshList[GEO_MODEL_AYAMCAN]->textureID = LoadTGA("Image//Can2_Tex.tga");

	meshList[GEO_MODEL_CHICKENSTOCK] = MeshBuilder::GenerateOBJ("model1", "OBJ//chickenstock.obj");
	meshList[GEO_MODEL_CHICKENSTOCK]->textureID = LoadTGA("Image//Packet_Tex.tga");

	meshList[GEO_MODEL_TOBLERONE] = MeshBuilder::GenerateOBJ("model1", "OBJ//toblerone.obj");
	meshList[GEO_MODEL_TOBLERONE]->textureID = LoadTGA("Image//Packet2_Tex.tga");

	meshList[GEO_MODEL_VEGCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//vegcan.obj");
	meshList[GEO_MODEL_VEGCAN]->textureID = LoadTGA("Image//vegcan.tga");

	meshList[GEO_MODEL_MILO] = MeshBuilder::GenerateOBJ("model1", "OBJ//milo.obj");
	meshList[GEO_MODEL_MILO]->textureID = LoadTGA("Image//milo.tga");

	meshList[GEO_MODEL_SARDCAN] = MeshBuilder::GenerateOBJ("model1", "OBJ//sardcan.obj");
	meshList[GEO_MODEL_SARDCAN]->textureID = LoadTGA("Image//sardcan.tga");

	meshList[GEO_MODEL_CACTUS] = MeshBuilder::GenerateOBJ("model1", "OBJ//cactus.obj");
	meshList[GEO_MODEL_CACTUS]->textureID = LoadTGA("Image//cactus.tga");

	meshList[GEO_MODEL_REDITOS] = MeshBuilder::GenerateOBJ("model1", "OBJ//reditos.obj");
	meshList[GEO_MODEL_REDITOS]->textureID = LoadTGA("Image//reditos.tga");

	meshList[GEO_MODEL_DEWITOS] = MeshBuilder::GenerateOBJ("model1", "OBJ//dewitos.obj");
	meshList[GEO_MODEL_DEWITOS]->textureID = LoadTGA("Image//dewitos.tga");

	meshList[GEO_MODEL_DIABETOS] = MeshBuilder::GenerateOBJ("model1", "OBJ//diabetos.obj");
	meshList[GEO_MODEL_DIABETOS]->textureID = LoadTGA("Image//diabetos.tga");

	meshList[GEO_MODEL_MOATIES] = MeshBuilder::GenerateOBJ("model1", "OBJ//moaties.obj");
	meshList[GEO_MODEL_MOATIES]->textureID = LoadTGA("Image//moaties.tga");

	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//comic.tga");

	EntranceDoorSlide = data.GetRenderPos(1)->getTranslationX();
	ExitDoorSlide = data.GetRenderPos(2)->getTranslationX();

	npc.setName(character.GetRenderPos(0)->getName());
	npc.setPosX(character.GetRenderPos(0)->getTranslationX());
	npc.setPosZ(character.GetRenderPos(0)->getTranslationZ());
	shopper.setPosX(-75);
	shopper.setPosZ(25);
}



static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;
bool Lightsssss = false;

void SP2::Update(double dt)
{
	float LSPEED = 10.f;
	
	
	if(Application::IsKeyPressed('1')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	if(Application::IsKeyPressed('I'))
		lights[0].position.z -= (float)(LSPEED * dt);
	if(Application::IsKeyPressed('K'))
		lights[0].position.z += (float)(LSPEED * dt);
	if(Application::IsKeyPressed('J'))
		lights[0].position.x -= (float)(LSPEED * dt);
	if(Application::IsKeyPressed('L'))
		lights[0].position.x += (float)(LSPEED * dt);
	if(Application::IsKeyPressed('O'))
		lights[0].position.y -= (float)(LSPEED * dt);
	if(Application::IsKeyPressed('P'))
		lights[0].position.y += (float)(LSPEED * dt);

	if(Application::IsKeyPressed('Q'))
		lights[1].position.z -= (float)(LSPEED * dt);
	if(Application::IsKeyPressed('E'))
		lights[1].position.z += (float)(LSPEED * dt);
	if(Application::IsKeyPressed('R'))
		lights[1].position.x -= (float)(LSPEED * dt);
	if(Application::IsKeyPressed('T'))
		lights[1].position.x += (float)(LSPEED * dt);
	if(Application::IsKeyPressed('Y'))
		lights[1].position.y -= (float)(LSPEED * dt);
	if(Application::IsKeyPressed('U'))
		lights[1].position.y += (float)(LSPEED * dt);
	
	if(Application::IsKeyPressed('F'))
	{
		Lightsssss = true;
	}
	if(Application::IsKeyPressed('G'))
	{
		Lightsssss = false;
	}

	if(Application::IsKeyPressed('Z'))
	{
		//to do: switch light type to POINT and pass the information to shader
		lights[0].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	if(Application::IsKeyPressed('X'))
	{
		//to do: switch light type to DIRECTIONAL and pass the information to shader
		lights[0].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}
	if(Application::IsKeyPressed('C'))
	{
		//to do: switch light type to SPOT and pass the information to shader
		lights[0].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	}

	if(Application::IsKeyPressed('V'))
	{
		//to do: switch light type to POINT and pass the information to shader
		lights[1].type = Light::LIGHT_POINT;
		glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	}
	if(Application::IsKeyPressed('B'))
	{
		//to do: switch light type to DIRECTIONAL and pass the information to shader
		lights[1].type = Light::LIGHT_DIRECTIONAL;
		glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	}
	if(Application::IsKeyPressed('N'))
	{
		//to do: switch light type to SPOT and pass the information to shader
		lights[1].type = Light::LIGHT_SPOT;
		glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	}

	if ( camera.EntranceDoor == true)
	{
		if ( EntranceDoorSlide >= 53)
		{
				EntranceDoorSlide-= 0.4;
		}
	}
	else if ( camera.EntranceDoor == false)
	{
		if ( EntranceDoorSlide <= 66.4)
		{
				EntranceDoorSlide+= 0.4;
		}
	}
	

	if ( camera.ExitDoor == true)
	{
		if ( ExitDoorSlide <= -53)
		{
				ExitDoorSlide+= 0.4;
		}
	}
	else if ( camera.ExitDoor == false)
	{
		if ( ExitDoorSlide >= -66.4)
		{
				ExitDoorSlide-= 0.4;
		}
	}

	npc.TestAIPath();
	shopper.ShopPathing();

	std::stringstream dd;
	dd << camera.position.x;
	XPos = dd.str();

	std::stringstream ff;
	ff << camera.position.z;
	ZPos = ff.str();

	FPS = 1/dt;
	std::stringstream ss;
	ss << FPS;
	FPS_count = ss.str();
	camera.Update(dt);
}

void SP2::Render()
{
	//bool toggleLight = false;

	//clear depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Temp variables
	Mtx44 MVP;

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	if(lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(lights[1].position.x, lights[1].position.y, lights[1].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL2], false);
	modelStack.PopMatrix();

	RenderMesh(meshList[GEO_AXES], false);
	modelStack.PushMatrix();

	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	RenderWorld();
	RenderCashier();
	RenderFridge();
	RenderShelves();
	RenderCharacter();
	RenderCereal();
	RenderDrinks();
	RenderChocolate();
	RenderSkybox();
	RenderChips();
	RenderPizza();
	RenderIceCream();
	RenderMcNCheese();
	RenderSardines();
	RenderAyamCan();
	RenderChickenStock();
	RenderToblerone();
	RenderMilo();
	RenderVegCan();
	RenderSardCan();
	RenderCactus();
	RenderCanFood();
	RenderReditos();
	RenderDewitos();
	RenderDiabetos();
	RenderMOaties();

	RenderTextOnScreen(meshList[GEO_TEXT], "FPS: " , Color(0, 1, 0), 3, 14, 18);
	RenderTextOnScreen(meshList[GEO_TEXT], FPS_count , Color(0, 1, 0), 3, 18, 18);

	RenderTextOnScreen(meshList[GEO_TEXT], XPos , Color(0, 1, 0), 3, 0, 18);
	RenderTextOnScreen(meshList[GEO_TEXT], ZPos , Color(0, 1, 0), 3, 0, 17);
	
}

void SP2::RenderSkybox()
{
	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Scale(1000,1000,1000);
	modelStack.Translate(0,0.1,-0.497);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Scale(1000,1000,1000);
	modelStack.Translate(0.497,0.1,0);
	modelStack.Rotate(-90,0,1,0);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Scale(1000,1000,1000);
	modelStack.Translate(-0.497,0.1,0);
	modelStack.Rotate(90,0,1,0);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Scale(1000,1000,1000);
	modelStack.Translate(0,0.1,0.497);
	modelStack.Rotate(180,0,1,0);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Scale(1000,1000,1000);
	modelStack.Translate(0,0.597,0);
	modelStack.Rotate(90,1,0,0);
	modelStack.PushMatrix();
	modelStack.Rotate(90,0,0,1);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Scale(1000,1000,1000);
	modelStack.Translate(0,0,0);
	modelStack.Rotate(-90,1,0,0);
	modelStack.PushMatrix();
	modelStack.Rotate(90,0,0,1);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
	modelStack.PopMatrix();


}

void SP2::RenderCashier()
{
		for(int i = 0; i < 4; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(cashier.GetRenderPos(0)->getTranslationX(),cashier.GetRenderPos(0)->getTranslationY(),cashier.GetRenderPos(0)->getTranslationZ()+(i*16));
		modelStack.Rotate(cashier.GetRenderPos(0)->getRotation(),cashier.GetRenderPos(0)->getRX(),cashier.GetRenderPos(0)->getRY(),cashier.GetRenderPos(0)->getRZ());
		modelStack.Scale(cashier.GetRenderPos(0)->getScaleX(),cashier.GetRenderPos(0)->getScaleY(),cashier.GetRenderPos(0)->getScaleZ());
		RenderMesh(meshList[GEO_MODEL_CASHIER], true);
		modelStack.PopMatrix();
	}
}

void SP2::RenderFridge()
{
	for(int i = 0; i < 7; ++i)
	{
		modelStack.PushMatrix();
		modelStack.Translate(fridge.GetRenderPos(0)->getTranslationX()-(i*14),fridge.GetRenderPos(0)->getTranslationY(),fridge.GetRenderPos(0)->getTranslationZ());
		modelStack.Rotate(fridge.GetRenderPos(0)->getRotation(),fridge.GetRenderPos(0)->getRX(),fridge.GetRenderPos(0)->getRY(),fridge.GetRenderPos(0)->getRZ());
		modelStack.Scale(fridge.GetRenderPos(0)->getScaleX(),fridge.GetRenderPos(0)->getScaleY(),fridge.GetRenderPos(0)->getScaleZ());
		RenderMesh(meshList[GEO_MODEL_FRIDGE], true);
		modelStack.PopMatrix();
	}
}

void SP2::RenderShelves()
{
	int temp = 0;
	for(int i = 1; i <= 3; ++ i )
	{
		for(int j = 0; j < 12; ++ j )
		{
			if ( j >= 4 && j < 8 )
			{
			 temp = 10;
			}
			else  if ( j >= 8 )
			{
			temp = 20;
			}
			else
			{
				temp = 0;
			}
			
			modelStack.PushMatrix();
			modelStack.Translate(shelve.GetRenderPos(0)->getTranslationX()+(j*6)+temp,shelve.GetRenderPos(0)->getTranslationY(),shelve.GetRenderPos(0)->getTranslationZ()+(i*15));
			modelStack.Rotate(shelve.GetRenderPos(0)->getRotation(),shelve.GetRenderPos(0)->getRX(),shelve.GetRenderPos(0)->getRY(),shelve.GetRenderPos(0)->getRZ());
			modelStack.Scale(shelve.GetRenderPos(0)->getScaleX(),shelve.GetRenderPos(0)->getScaleY(),shelve.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_SHELF], true);
			modelStack.PopMatrix();
		}
	}
}

void SP2::RenderVegCan()
{
	//Left Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(vegcan.GetRenderPos(0)->getTranslationX()+(j*1.4),vegcan.GetRenderPos(0)->getTranslationY(),vegcan.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(vegcan.GetRenderPos(0)->getRotation(),vegcan.GetRenderPos(0)->getRX(),vegcan.GetRenderPos(0)->getRY(),vegcan.GetRenderPos(0)->getRZ());
			modelStack.Scale(vegcan.GetRenderPos(0)->getScaleX(),vegcan.GetRenderPos(0)->getScaleY(),vegcan.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_VEGCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(vegcan.GetRenderPos(1)->getTranslationX()+(j*1.4),vegcan.GetRenderPos(1)->getTranslationY(),vegcan.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(vegcan.GetRenderPos(1)->getRotation(),vegcan.GetRenderPos(1)->getRX(),vegcan.GetRenderPos(1)->getRY(),vegcan.GetRenderPos(1)->getRZ());
			modelStack.Scale(vegcan.GetRenderPos(1)->getScaleX(),vegcan.GetRenderPos(1)->getScaleY(),vegcan.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_VEGCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(vegcan.GetRenderPos(2)->getTranslationX()+(j*1.4),vegcan.GetRenderPos(2)->getTranslationY(),vegcan.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(vegcan.GetRenderPos(2)->getRotation(),vegcan.GetRenderPos(2)->getRX(),vegcan.GetRenderPos(2)->getRY(),vegcan.GetRenderPos(0)->getRZ());
			modelStack.Scale(vegcan.GetRenderPos(2)->getScaleX(),vegcan.GetRenderPos(2)->getScaleY(),vegcan.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_VEGCAN], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(vegcan.GetRenderPos(3)->getTranslationX()+(j*1.4),vegcan.GetRenderPos(3)->getTranslationY(),vegcan.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(vegcan.GetRenderPos(3)->getRotation(),vegcan.GetRenderPos(3)->getRX(),vegcan.GetRenderPos(3)->getRY(),vegcan.GetRenderPos(3)->getRZ());
			modelStack.Scale(vegcan.GetRenderPos(3)->getScaleX(),vegcan.GetRenderPos(3)->getScaleY(),vegcan.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_VEGCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(vegcan.GetRenderPos(4)->getTranslationX()+(j*1.4),vegcan.GetRenderPos(4)->getTranslationY(),vegcan.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(vegcan.GetRenderPos(4)->getRotation(),vegcan.GetRenderPos(4)->getRX(),vegcan.GetRenderPos(4)->getRY(),vegcan.GetRenderPos(4)->getRZ());
			modelStack.Scale(vegcan.GetRenderPos(4)->getScaleX(),vegcan.GetRenderPos(4)->getScaleY(),vegcan.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_VEGCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(vegcan.GetRenderPos(5)->getTranslationX()+(j*1.4),vegcan.GetRenderPos(5)->getTranslationY(),vegcan.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(vegcan.GetRenderPos(5)->getRotation(),vegcan.GetRenderPos(5)->getRX(),vegcan.GetRenderPos(5)->getRY(),vegcan.GetRenderPos(5)->getRZ());
			modelStack.Scale(vegcan.GetRenderPos(5)->getScaleX(),vegcan.GetRenderPos(5)->getScaleY(),vegcan.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_VEGCAN], true);
			modelStack.PopMatrix();
		}
	}

}

void SP2::RenderMilo()
{
	//Left Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(milo.GetRenderPos(0)->getTranslationX()+(j*1.4),milo.GetRenderPos(0)->getTranslationY(),milo.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(milo.GetRenderPos(0)->getRotation(),milo.GetRenderPos(0)->getRX(),milo.GetRenderPos(0)->getRY(),milo.GetRenderPos(0)->getRZ());
			modelStack.Scale(milo.GetRenderPos(0)->getScaleX(),milo.GetRenderPos(0)->getScaleY(),milo.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MILO], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(milo.GetRenderPos(1)->getTranslationX()+(j*1.4),milo.GetRenderPos(1)->getTranslationY(),milo.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(milo.GetRenderPos(1)->getRotation(),milo.GetRenderPos(1)->getRX(),milo.GetRenderPos(1)->getRY(),milo.GetRenderPos(1)->getRZ());
			modelStack.Scale(milo.GetRenderPos(1)->getScaleX(),milo.GetRenderPos(1)->getScaleY(),milo.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MILO], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(milo.GetRenderPos(2)->getTranslationX()+(j*1.4),milo.GetRenderPos(2)->getTranslationY(),milo.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(milo.GetRenderPos(2)->getRotation(),milo.GetRenderPos(2)->getRX(),milo.GetRenderPos(2)->getRY(),milo.GetRenderPos(0)->getRZ());
			modelStack.Scale(milo.GetRenderPos(2)->getScaleX(),milo.GetRenderPos(2)->getScaleY(),milo.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MILO], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(milo.GetRenderPos(3)->getTranslationX()+(j*1.4),milo.GetRenderPos(3)->getTranslationY(),milo.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(milo.GetRenderPos(3)->getRotation(),milo.GetRenderPos(3)->getRX(),milo.GetRenderPos(3)->getRY(),milo.GetRenderPos(3)->getRZ());
			modelStack.Scale(milo.GetRenderPos(3)->getScaleX(),milo.GetRenderPos(3)->getScaleY(),milo.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MILO], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(milo.GetRenderPos(4)->getTranslationX()+(j*1.4),milo.GetRenderPos(4)->getTranslationY(),milo.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(milo.GetRenderPos(4)->getRotation(),milo.GetRenderPos(4)->getRX(),milo.GetRenderPos(4)->getRY(),milo.GetRenderPos(4)->getRZ());
			modelStack.Scale(milo.GetRenderPos(4)->getScaleX(),milo.GetRenderPos(4)->getScaleY(),milo.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MILO], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(milo.GetRenderPos(5)->getTranslationX()+(j*1.4),milo.GetRenderPos(5)->getTranslationY(),milo.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(milo.GetRenderPos(5)->getRotation(),milo.GetRenderPos(5)->getRX(),milo.GetRenderPos(5)->getRY(),milo.GetRenderPos(5)->getRZ());
			modelStack.Scale(milo.GetRenderPos(5)->getScaleX(),milo.GetRenderPos(5)->getScaleY(),milo.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MILO], true);
			modelStack.PopMatrix();
		}
	}

}

void SP2::RenderSardCan()
{
	//Left Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(sardcan.GetRenderPos(0)->getTranslationX()+(j*1.4),sardcan.GetRenderPos(0)->getTranslationY(),sardcan.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(sardcan.GetRenderPos(0)->getRotation(),sardcan.GetRenderPos(0)->getRX(),sardcan.GetRenderPos(0)->getRY(),sardcan.GetRenderPos(0)->getRZ());
			modelStack.Scale(sardcan.GetRenderPos(0)->getScaleX(),sardcan.GetRenderPos(0)->getScaleY(),sardcan.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_SARDCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(sardcan.GetRenderPos(1)->getTranslationX()+(j*1.4),sardcan.GetRenderPos(1)->getTranslationY(),sardcan.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(sardcan.GetRenderPos(1)->getRotation(),sardcan.GetRenderPos(1)->getRX(),sardcan.GetRenderPos(1)->getRY(),sardcan.GetRenderPos(1)->getRZ());
			modelStack.Scale(sardcan.GetRenderPos(1)->getScaleX(),sardcan.GetRenderPos(1)->getScaleY(),sardcan.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_SARDCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(sardcan.GetRenderPos(2)->getTranslationX()+(j*1.4),sardcan.GetRenderPos(2)->getTranslationY(),sardcan.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(sardcan.GetRenderPos(2)->getRotation(),sardcan.GetRenderPos(2)->getRX(),sardcan.GetRenderPos(2)->getRY(),sardcan.GetRenderPos(0)->getRZ());
			modelStack.Scale(sardcan.GetRenderPos(2)->getScaleX(),sardcan.GetRenderPos(2)->getScaleY(),sardcan.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_SARDCAN], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(sardcan.GetRenderPos(3)->getTranslationX()+(j*1.4),sardcan.GetRenderPos(3)->getTranslationY(),sardcan.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(sardcan.GetRenderPos(3)->getRotation(),sardcan.GetRenderPos(3)->getRX(),sardcan.GetRenderPos(3)->getRY(),sardcan.GetRenderPos(3)->getRZ());
			modelStack.Scale(sardcan.GetRenderPos(3)->getScaleX(),sardcan.GetRenderPos(3)->getScaleY(),sardcan.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_SARDCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(sardcan.GetRenderPos(4)->getTranslationX()+(j*1.4),sardcan.GetRenderPos(4)->getTranslationY(),sardcan.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(sardcan.GetRenderPos(4)->getRotation(),sardcan.GetRenderPos(4)->getRX(),sardcan.GetRenderPos(4)->getRY(),sardcan.GetRenderPos(4)->getRZ());
			modelStack.Scale(sardcan.GetRenderPos(4)->getScaleX(),sardcan.GetRenderPos(4)->getScaleY(),sardcan.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_SARDCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(sardcan.GetRenderPos(5)->getTranslationX()+(j*1.4),sardcan.GetRenderPos(5)->getTranslationY(),sardcan.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(sardcan.GetRenderPos(5)->getRotation(),sardcan.GetRenderPos(5)->getRX(),sardcan.GetRenderPos(5)->getRY(),sardcan.GetRenderPos(5)->getRZ());
			modelStack.Scale(sardcan.GetRenderPos(5)->getScaleX(),sardcan.GetRenderPos(5)->getScaleY(),sardcan.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_SARDCAN], true);
			modelStack.PopMatrix();
		}
	}

}

void SP2::RenderCactus()
{
	//Left Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cactus.GetRenderPos(0)->getTranslationX()+(j*1.4),cactus.GetRenderPos(0)->getTranslationY(),cactus.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cactus.GetRenderPos(0)->getRotation(),cactus.GetRenderPos(0)->getRX(),cactus.GetRenderPos(0)->getRY(),cactus.GetRenderPos(0)->getRZ());
			modelStack.Scale(cactus.GetRenderPos(0)->getScaleX(),cactus.GetRenderPos(0)->getScaleY(),cactus.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CACTUS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cactus.GetRenderPos(1)->getTranslationX()+(j*1.4),cactus.GetRenderPos(1)->getTranslationY(),cactus.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cactus.GetRenderPos(1)->getRotation(),cactus.GetRenderPos(1)->getRX(),cactus.GetRenderPos(1)->getRY(),cactus.GetRenderPos(1)->getRZ());
			modelStack.Scale(cactus.GetRenderPos(1)->getScaleX(),cactus.GetRenderPos(1)->getScaleY(),cactus.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CACTUS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cactus.GetRenderPos(2)->getTranslationX()+(j*1.4),cactus.GetRenderPos(2)->getTranslationY(),cactus.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cactus.GetRenderPos(2)->getRotation(),cactus.GetRenderPos(2)->getRX(),cactus.GetRenderPos(2)->getRY(),cactus.GetRenderPos(0)->getRZ());
			modelStack.Scale(cactus.GetRenderPos(2)->getScaleX(),cactus.GetRenderPos(2)->getScaleY(),cactus.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CACTUS], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cactus.GetRenderPos(3)->getTranslationX()+(j*1.4),cactus.GetRenderPos(3)->getTranslationY(),cactus.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cactus.GetRenderPos(3)->getRotation(),cactus.GetRenderPos(3)->getRX(),cactus.GetRenderPos(3)->getRY(),cactus.GetRenderPos(3)->getRZ());
			modelStack.Scale(cactus.GetRenderPos(3)->getScaleX(),cactus.GetRenderPos(3)->getScaleY(),cactus.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CACTUS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cactus.GetRenderPos(4)->getTranslationX()+(j*1.4),cactus.GetRenderPos(4)->getTranslationY(),cactus.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cactus.GetRenderPos(4)->getRotation(),cactus.GetRenderPos(4)->getRX(),cactus.GetRenderPos(4)->getRY(),cactus.GetRenderPos(4)->getRZ());
			modelStack.Scale(cactus.GetRenderPos(4)->getScaleX(),cactus.GetRenderPos(4)->getScaleY(),cactus.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CACTUS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cactus.GetRenderPos(5)->getTranslationX()+(j*1.4),cactus.GetRenderPos(5)->getTranslationY(),cactus.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cactus.GetRenderPos(5)->getRotation(),cactus.GetRenderPos(5)->getRX(),cactus.GetRenderPos(5)->getRY(),cactus.GetRenderPos(5)->getRZ());
			modelStack.Scale(cactus.GetRenderPos(5)->getScaleX(),cactus.GetRenderPos(5)->getScaleY(),cactus.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CACTUS], true);
			modelStack.PopMatrix();
		}
	}

}

void SP2::RenderReditos()
{
	//Left Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(reditos.GetRenderPos(0)->getTranslationX()+(j*1.4),reditos.GetRenderPos(0)->getTranslationY(),reditos.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(reditos.GetRenderPos(0)->getRotation(),reditos.GetRenderPos(0)->getRX(),reditos.GetRenderPos(0)->getRY(),reditos.GetRenderPos(0)->getRZ());
			modelStack.Scale(reditos.GetRenderPos(0)->getScaleX(),reditos.GetRenderPos(0)->getScaleY(),reditos.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_REDITOS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(reditos.GetRenderPos(1)->getTranslationX()+(j*1.4),reditos.GetRenderPos(1)->getTranslationY(),reditos.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(reditos.GetRenderPos(1)->getRotation(),reditos.GetRenderPos(1)->getRX(),reditos.GetRenderPos(1)->getRY(),reditos.GetRenderPos(1)->getRZ());
			modelStack.Scale(reditos.GetRenderPos(1)->getScaleX(),reditos.GetRenderPos(1)->getScaleY(),reditos.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_REDITOS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(reditos.GetRenderPos(2)->getTranslationX()+(j*1.4),reditos.GetRenderPos(2)->getTranslationY(),reditos.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(reditos.GetRenderPos(2)->getRotation(),reditos.GetRenderPos(2)->getRX(),reditos.GetRenderPos(2)->getRY(),reditos.GetRenderPos(0)->getRZ());
			modelStack.Scale(reditos.GetRenderPos(2)->getScaleX(),reditos.GetRenderPos(2)->getScaleY(),reditos.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_REDITOS], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(reditos.GetRenderPos(3)->getTranslationX()+(j*1.4),reditos.GetRenderPos(3)->getTranslationY(),reditos.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(reditos.GetRenderPos(3)->getRotation(),reditos.GetRenderPos(3)->getRX(),reditos.GetRenderPos(3)->getRY(),reditos.GetRenderPos(3)->getRZ());
			modelStack.Scale(reditos.GetRenderPos(3)->getScaleX(),reditos.GetRenderPos(3)->getScaleY(),reditos.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_REDITOS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(reditos.GetRenderPos(4)->getTranslationX()+(j*1.4),reditos.GetRenderPos(4)->getTranslationY(),reditos.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(reditos.GetRenderPos(4)->getRotation(),reditos.GetRenderPos(4)->getRX(),reditos.GetRenderPos(4)->getRY(),reditos.GetRenderPos(4)->getRZ());
			modelStack.Scale(reditos.GetRenderPos(4)->getScaleX(),reditos.GetRenderPos(4)->getScaleY(),reditos.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_REDITOS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(reditos.GetRenderPos(5)->getTranslationX()+(j*1.4),reditos.GetRenderPos(5)->getTranslationY(),reditos.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(reditos.GetRenderPos(5)->getRotation(),reditos.GetRenderPos(5)->getRX(),reditos.GetRenderPos(5)->getRY(),reditos.GetRenderPos(5)->getRZ());
			modelStack.Scale(reditos.GetRenderPos(5)->getScaleX(),reditos.GetRenderPos(5)->getScaleY(),reditos.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_REDITOS], true);
			modelStack.PopMatrix();
		}
	}

}

void SP2::RenderDewitos()
{
	//Left Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(dewitos.GetRenderPos(0)->getTranslationX()+(j*1.4),dewitos.GetRenderPos(0)->getTranslationY(),dewitos.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(dewitos.GetRenderPos(0)->getRotation(),dewitos.GetRenderPos(0)->getRX(),dewitos.GetRenderPos(0)->getRY(),dewitos.GetRenderPos(0)->getRZ());
			modelStack.Scale(dewitos.GetRenderPos(0)->getScaleX(),dewitos.GetRenderPos(0)->getScaleY(),dewitos.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_DEWITOS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(dewitos.GetRenderPos(1)->getTranslationX()+(j*1.4),dewitos.GetRenderPos(1)->getTranslationY(),dewitos.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(dewitos.GetRenderPos(1)->getRotation(),dewitos.GetRenderPos(1)->getRX(),dewitos.GetRenderPos(1)->getRY(),dewitos.GetRenderPos(1)->getRZ());
			modelStack.Scale(dewitos.GetRenderPos(1)->getScaleX(),dewitos.GetRenderPos(1)->getScaleY(),dewitos.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_DEWITOS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(dewitos.GetRenderPos(2)->getTranslationX()+(j*1.4),dewitos.GetRenderPos(2)->getTranslationY(),dewitos.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(dewitos.GetRenderPos(2)->getRotation(),dewitos.GetRenderPos(2)->getRX(),dewitos.GetRenderPos(2)->getRY(),dewitos.GetRenderPos(0)->getRZ());
			modelStack.Scale(dewitos.GetRenderPos(2)->getScaleX(),dewitos.GetRenderPos(2)->getScaleY(),dewitos.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_DEWITOS], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(dewitos.GetRenderPos(3)->getTranslationX()+(j*1.4),dewitos.GetRenderPos(3)->getTranslationY(),dewitos.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(dewitos.GetRenderPos(3)->getRotation(),dewitos.GetRenderPos(3)->getRX(),dewitos.GetRenderPos(3)->getRY(),dewitos.GetRenderPos(3)->getRZ());
			modelStack.Scale(dewitos.GetRenderPos(3)->getScaleX(),dewitos.GetRenderPos(3)->getScaleY(),dewitos.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_DEWITOS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(dewitos.GetRenderPos(4)->getTranslationX()+(j*1.4),dewitos.GetRenderPos(4)->getTranslationY(),dewitos.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(dewitos.GetRenderPos(4)->getRotation(),dewitos.GetRenderPos(4)->getRX(),dewitos.GetRenderPos(4)->getRY(),dewitos.GetRenderPos(4)->getRZ());
			modelStack.Scale(dewitos.GetRenderPos(4)->getScaleX(),dewitos.GetRenderPos(4)->getScaleY(),dewitos.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_DEWITOS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(dewitos.GetRenderPos(5)->getTranslationX()+(j*1.4),dewitos.GetRenderPos(5)->getTranslationY(),dewitos.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(dewitos.GetRenderPos(5)->getRotation(),dewitos.GetRenderPos(5)->getRX(),dewitos.GetRenderPos(5)->getRY(),dewitos.GetRenderPos(5)->getRZ());
			modelStack.Scale(dewitos.GetRenderPos(5)->getScaleX(),dewitos.GetRenderPos(5)->getScaleY(),dewitos.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_DEWITOS], true);
			modelStack.PopMatrix();
		}
	}

}

void SP2::RenderDiabetos()
{
	//Left Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(diabetos.GetRenderPos(0)->getTranslationX()+(j*1.4),diabetos.GetRenderPos(0)->getTranslationY(),diabetos.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(diabetos.GetRenderPos(0)->getRotation(),diabetos.GetRenderPos(0)->getRX(),diabetos.GetRenderPos(0)->getRY(),diabetos.GetRenderPos(0)->getRZ());
			modelStack.Scale(diabetos.GetRenderPos(0)->getScaleX(),diabetos.GetRenderPos(0)->getScaleY(),diabetos.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_DIABETOS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(diabetos.GetRenderPos(1)->getTranslationX()+(j*1.4),diabetos.GetRenderPos(1)->getTranslationY(),diabetos.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(diabetos.GetRenderPos(1)->getRotation(),diabetos.GetRenderPos(1)->getRX(),diabetos.GetRenderPos(1)->getRY(),diabetos.GetRenderPos(1)->getRZ());
			modelStack.Scale(diabetos.GetRenderPos(1)->getScaleX(),diabetos.GetRenderPos(1)->getScaleY(),diabetos.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_DIABETOS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(diabetos.GetRenderPos(2)->getTranslationX()+(j*1.4),diabetos.GetRenderPos(2)->getTranslationY(),diabetos.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(diabetos.GetRenderPos(2)->getRotation(),diabetos.GetRenderPos(2)->getRX(),diabetos.GetRenderPos(2)->getRY(),diabetos.GetRenderPos(0)->getRZ());
			modelStack.Scale(diabetos.GetRenderPos(2)->getScaleX(),diabetos.GetRenderPos(2)->getScaleY(),diabetos.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_DIABETOS], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(diabetos.GetRenderPos(3)->getTranslationX()+(j*1.4),diabetos.GetRenderPos(3)->getTranslationY(),diabetos.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(diabetos.GetRenderPos(3)->getRotation(),diabetos.GetRenderPos(3)->getRX(),diabetos.GetRenderPos(3)->getRY(),diabetos.GetRenderPos(3)->getRZ());
			modelStack.Scale(diabetos.GetRenderPos(3)->getScaleX(),diabetos.GetRenderPos(3)->getScaleY(),diabetos.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_DIABETOS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(diabetos.GetRenderPos(4)->getTranslationX()+(j*1.4),diabetos.GetRenderPos(4)->getTranslationY(),diabetos.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(diabetos.GetRenderPos(4)->getRotation(),diabetos.GetRenderPos(4)->getRX(),diabetos.GetRenderPos(4)->getRY(),diabetos.GetRenderPos(4)->getRZ());
			modelStack.Scale(diabetos.GetRenderPos(4)->getScaleX(),diabetos.GetRenderPos(4)->getScaleY(),diabetos.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_DIABETOS], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(diabetos.GetRenderPos(5)->getTranslationX()+(j*1.4),diabetos.GetRenderPos(5)->getTranslationY(),diabetos.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(diabetos.GetRenderPos(5)->getRotation(),diabetos.GetRenderPos(5)->getRX(),diabetos.GetRenderPos(5)->getRY(),diabetos.GetRenderPos(5)->getRZ());
			modelStack.Scale(diabetos.GetRenderPos(5)->getScaleX(),diabetos.GetRenderPos(5)->getScaleY(),diabetos.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_DIABETOS], true);
			modelStack.PopMatrix();
		}
	}

}

void SP2::RenderMOaties()
{
	//Left Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(moaties.GetRenderPos(0)->getTranslationX()+(j*1.4),moaties.GetRenderPos(0)->getTranslationY(),moaties.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(moaties.GetRenderPos(0)->getRotation(),moaties.GetRenderPos(0)->getRX(),moaties.GetRenderPos(0)->getRY(),moaties.GetRenderPos(0)->getRZ());
			modelStack.Scale(moaties.GetRenderPos(0)->getScaleX(),moaties.GetRenderPos(0)->getScaleY(),moaties.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MOATIES], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )

	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(moaties.GetRenderPos(1)->getTranslationX()+(j*1.4),moaties.GetRenderPos(1)->getTranslationY(),moaties.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(moaties.GetRenderPos(1)->getRotation(),moaties.GetRenderPos(1)->getRX(),moaties.GetRenderPos(1)->getRY(),moaties.GetRenderPos(1)->getRZ());
			modelStack.Scale(moaties.GetRenderPos(1)->getScaleX(),moaties.GetRenderPos(1)->getScaleY(),moaties.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MOATIES], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(moaties.GetRenderPos(2)->getTranslationX()+(j*1.4),moaties.GetRenderPos(2)->getTranslationY(),moaties.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(moaties.GetRenderPos(2)->getRotation(),moaties.GetRenderPos(2)->getRX(),moaties.GetRenderPos(2)->getRY(),moaties.GetRenderPos(0)->getRZ());
			modelStack.Scale(moaties.GetRenderPos(2)->getScaleX(),moaties.GetRenderPos(2)->getScaleY(),moaties.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MOATIES], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(moaties.GetRenderPos(3)->getTranslationX()+(j*1.4),moaties.GetRenderPos(3)->getTranslationY(),moaties.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(moaties.GetRenderPos(3)->getRotation(),moaties.GetRenderPos(3)->getRX(),moaties.GetRenderPos(3)->getRY(),moaties.GetRenderPos(3)->getRZ());
			modelStack.Scale(moaties.GetRenderPos(3)->getScaleX(),moaties.GetRenderPos(3)->getScaleY(),moaties.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MOATIES], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(moaties.GetRenderPos(4)->getTranslationX()+(j*1.4),moaties.GetRenderPos(4)->getTranslationY(),moaties.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(moaties.GetRenderPos(4)->getRotation(),moaties.GetRenderPos(4)->getRX(),moaties.GetRenderPos(4)->getRY(),moaties.GetRenderPos(4)->getRZ());
			modelStack.Scale(moaties.GetRenderPos(4)->getScaleX(),moaties.GetRenderPos(4)->getScaleY(),moaties.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MOATIES], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(moaties.GetRenderPos(5)->getTranslationX()+(j*1.4),moaties.GetRenderPos(5)->getTranslationY(),moaties.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(moaties.GetRenderPos(5)->getRotation(),moaties.GetRenderPos(5)->getRX(),moaties.GetRenderPos(5)->getRY(),moaties.GetRenderPos(5)->getRZ());
			modelStack.Scale(moaties.GetRenderPos(5)->getScaleX(),moaties.GetRenderPos(5)->getScaleY(),moaties.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MOATIES], true);
			modelStack.PopMatrix();
		}
	}

}

void SP2::RenderCanFood()
{
	//Shelf 2-5
	//Left Side(BBCan)
	modelStack.PushMatrix();
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(0)->getTranslationX()+(j*1.4),canfood.GetRenderPos(0)->getTranslationY(),canfood.GetRenderPos(0)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(0)->getRotation(),canfood.GetRenderPos(0)->getRX(),canfood.GetRenderPos(0)->getRY(),canfood.GetRenderPos(0)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(0)->getScaleX(),canfood.GetRenderPos(0)->getScaleY(),canfood.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_BBCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(1)->getTranslationX()+(j*1.4),canfood.GetRenderPos(1)->getTranslationY(),canfood.GetRenderPos(1)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(1)->getRotation(),canfood.GetRenderPos(1)->getRX(),canfood.GetRenderPos(1)->getRY(),canfood.GetRenderPos(1)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(1)->getScaleX(),canfood.GetRenderPos(1)->getScaleY(),canfood.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_BBCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(2)->getTranslationX()+(j*1.4),canfood.GetRenderPos(2)->getTranslationY(),canfood.GetRenderPos(2)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(2)->getRotation(),canfood.GetRenderPos(2)->getRX(),canfood.GetRenderPos(2)->getRY(),canfood.GetRenderPos(0)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(2)->getScaleX(),canfood.GetRenderPos(2)->getScaleY(),canfood.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_BBCAN], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side(BBCAN)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(3)->getTranslationX()+(j*1.4),canfood.GetRenderPos(3)->getTranslationY(),canfood.GetRenderPos(3)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(3)->getRotation(),canfood.GetRenderPos(3)->getRX(),canfood.GetRenderPos(3)->getRY(),canfood.GetRenderPos(3)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(3)->getScaleX(),canfood.GetRenderPos(3)->getScaleY(),canfood.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_BBCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(4)->getTranslationX()+(j*1.4),canfood.GetRenderPos(4)->getTranslationY(),canfood.GetRenderPos(4)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(4)->getRotation(),canfood.GetRenderPos(4)->getRX(),canfood.GetRenderPos(4)->getRY(),canfood.GetRenderPos(4)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(4)->getScaleX(),canfood.GetRenderPos(4)->getScaleY(),canfood.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_BBCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(5)->getTranslationX()+(j*1.4),canfood.GetRenderPos(5)->getTranslationY(),canfood.GetRenderPos(5)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(5)->getRotation(),canfood.GetRenderPos(5)->getRX(),canfood.GetRenderPos(5)->getRY(),canfood.GetRenderPos(5)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(5)->getScaleX(),canfood.GetRenderPos(5)->getScaleY(),canfood.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_BBCAN], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();

	//Shelf 2-6
	//LEFT Side(PUMPKINCAN)
	modelStack.PushMatrix();
	modelStack.Translate(6,0,0);
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(0)->getTranslationX()+(j*1.4),canfood.GetRenderPos(0)->getTranslationY(),canfood.GetRenderPos(0)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(0)->getRotation(),canfood.GetRenderPos(0)->getRX(),canfood.GetRenderPos(0)->getRY(),canfood.GetRenderPos(0)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(0)->getScaleX(),canfood.GetRenderPos(0)->getScaleY(),canfood.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PUMPKINCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(1)->getTranslationX()+(j*1.4),canfood.GetRenderPos(1)->getTranslationY(),canfood.GetRenderPos(1)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(1)->getRotation(),canfood.GetRenderPos(1)->getRX(),canfood.GetRenderPos(1)->getRY(),canfood.GetRenderPos(1)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(1)->getScaleX(),canfood.GetRenderPos(1)->getScaleY(),canfood.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PUMPKINCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(2)->getTranslationX()+(j*1.4),canfood.GetRenderPos(2)->getTranslationY(),canfood.GetRenderPos(2)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(2)->getRotation(),canfood.GetRenderPos(2)->getRX(),canfood.GetRenderPos(2)->getRY(),canfood.GetRenderPos(0)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(2)->getScaleX(),canfood.GetRenderPos(2)->getScaleY(),canfood.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PUMPKINCAN], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side(PUMPKINCAN)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(3)->getTranslationX()+(j*1.4),canfood.GetRenderPos(3)->getTranslationY(),canfood.GetRenderPos(3)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(3)->getRotation(),canfood.GetRenderPos(3)->getRX(),canfood.GetRenderPos(3)->getRY(),canfood.GetRenderPos(3)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(3)->getScaleX(),canfood.GetRenderPos(3)->getScaleY(),canfood.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PUMPKINCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(4)->getTranslationX()+(j*1.4),canfood.GetRenderPos(4)->getTranslationY(),canfood.GetRenderPos(4)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(4)->getRotation(),canfood.GetRenderPos(4)->getRX(),canfood.GetRenderPos(4)->getRY(),canfood.GetRenderPos(4)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(4)->getScaleX(),canfood.GetRenderPos(4)->getScaleY(),canfood.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PUMPKINCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(5)->getTranslationX()+(j*1.4),canfood.GetRenderPos(5)->getTranslationY(),canfood.GetRenderPos(5)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(5)->getRotation(),canfood.GetRenderPos(5)->getRX(),canfood.GetRenderPos(5)->getRY(),canfood.GetRenderPos(5)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(5)->getScaleX(),canfood.GetRenderPos(5)->getScaleY(),canfood.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PUMPKINCAN], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();

	//Shelf 2-7
	//LEFT Side(TOMATOSOUPCAN)
	modelStack.PushMatrix();
	modelStack.Translate(12,0,0);
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(0)->getTranslationX()+(j*1.4),canfood.GetRenderPos(0)->getTranslationY(),canfood.GetRenderPos(0)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(0)->getRotation(),canfood.GetRenderPos(0)->getRX(),canfood.GetRenderPos(0)->getRY(),canfood.GetRenderPos(0)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(0)->getScaleX(),canfood.GetRenderPos(0)->getScaleY(),canfood.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_TOMATOSOUPCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(1)->getTranslationX()+(j*1.4),canfood.GetRenderPos(1)->getTranslationY(),canfood.GetRenderPos(1)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(1)->getRotation(),canfood.GetRenderPos(1)->getRX(),canfood.GetRenderPos(1)->getRY(),canfood.GetRenderPos(1)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(1)->getScaleX(),canfood.GetRenderPos(1)->getScaleY(),canfood.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_TOMATOSOUPCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(2)->getTranslationX()+(j*1.4),canfood.GetRenderPos(2)->getTranslationY(),canfood.GetRenderPos(2)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(2)->getRotation(),canfood.GetRenderPos(2)->getRX(),canfood.GetRenderPos(2)->getRY(),canfood.GetRenderPos(0)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(2)->getScaleX(),canfood.GetRenderPos(2)->getScaleY(),canfood.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_TOMATOSOUPCAN], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side(TOMATOSOUPCAN)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(3)->getTranslationX()+(j*1.4),canfood.GetRenderPos(3)->getTranslationY(),canfood.GetRenderPos(3)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(3)->getRotation(),canfood.GetRenderPos(3)->getRX(),canfood.GetRenderPos(3)->getRY(),canfood.GetRenderPos(3)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(3)->getScaleX(),canfood.GetRenderPos(3)->getScaleY(),canfood.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_TOMATOSOUPCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(4)->getTranslationX()+(j*1.4),canfood.GetRenderPos(4)->getTranslationY(),canfood.GetRenderPos(4)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(4)->getRotation(),canfood.GetRenderPos(4)->getRX(),canfood.GetRenderPos(4)->getRY(),canfood.GetRenderPos(4)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(4)->getScaleX(),canfood.GetRenderPos(4)->getScaleY(),canfood.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_TOMATOSOUPCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(5)->getTranslationX()+(j*1.4),canfood.GetRenderPos(5)->getTranslationY(),canfood.GetRenderPos(5)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(5)->getRotation(),canfood.GetRenderPos(5)->getRX(),canfood.GetRenderPos(5)->getRY(),canfood.GetRenderPos(5)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(5)->getScaleX(),canfood.GetRenderPos(5)->getScaleY(),canfood.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_TOMATOSOUPCAN], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();

	//Shelf 2-8
	//LEFT Side(PEASNCARROTCAN)
	modelStack.PushMatrix();
	modelStack.Translate(18,0,0);
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(0)->getTranslationX()+(j*1.4),canfood.GetRenderPos(0)->getTranslationY(),canfood.GetRenderPos(0)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(0)->getRotation(),canfood.GetRenderPos(0)->getRX(),canfood.GetRenderPos(0)->getRY(),canfood.GetRenderPos(0)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(0)->getScaleX(),canfood.GetRenderPos(0)->getScaleY(),canfood.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEASNCARROTCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(1)->getTranslationX()+(j*1.4),canfood.GetRenderPos(1)->getTranslationY(),canfood.GetRenderPos(1)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(1)->getRotation(),canfood.GetRenderPos(1)->getRX(),canfood.GetRenderPos(1)->getRY(),canfood.GetRenderPos(1)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(1)->getScaleX(),canfood.GetRenderPos(1)->getScaleY(),canfood.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEASNCARROTCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(2)->getTranslationX()+(j*1.4),canfood.GetRenderPos(2)->getTranslationY(),canfood.GetRenderPos(2)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(2)->getRotation(),canfood.GetRenderPos(2)->getRX(),canfood.GetRenderPos(2)->getRY(),canfood.GetRenderPos(0)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(2)->getScaleX(),canfood.GetRenderPos(2)->getScaleY(),canfood.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEASNCARROTCAN], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side(PEASNCARROTCAN)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(3)->getTranslationX()+(j*1.4),canfood.GetRenderPos(3)->getTranslationY(),canfood.GetRenderPos(3)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(3)->getRotation(),canfood.GetRenderPos(3)->getRX(),canfood.GetRenderPos(3)->getRY(),canfood.GetRenderPos(3)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(3)->getScaleX(),canfood.GetRenderPos(3)->getScaleY(),canfood.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEASNCARROTCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(4)->getTranslationX()+(j*1.4),canfood.GetRenderPos(4)->getTranslationY(),canfood.GetRenderPos(4)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(4)->getRotation(),canfood.GetRenderPos(4)->getRX(),canfood.GetRenderPos(4)->getRY(),canfood.GetRenderPos(4)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(4)->getScaleX(),canfood.GetRenderPos(4)->getScaleY(),canfood.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEASNCARROTCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(canfood.GetRenderPos(5)->getTranslationX()+(j*1.4),canfood.GetRenderPos(5)->getTranslationY(),canfood.GetRenderPos(5)->getTranslationZ()+(i*0.8));
			modelStack.Rotate(canfood.GetRenderPos(5)->getRotation(),canfood.GetRenderPos(5)->getRX(),canfood.GetRenderPos(5)->getRY(),canfood.GetRenderPos(5)->getRZ());
			modelStack.Scale(canfood.GetRenderPos(5)->getScaleX(),canfood.GetRenderPos(5)->getScaleY(),canfood.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEASNCARROTCAN], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
}

void SP2::RenderDrinks()
{
	//Shelf 1-5 to 1-8
	//Left Side(Pepsi)
	modelStack.PushMatrix();
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(pepsi.GetRenderPos(0)->getTranslationX()+(j*1.4),pepsi.GetRenderPos(0)->getTranslationY(),pepsi.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(pepsi.GetRenderPos(0)->getRotation(),pepsi.GetRenderPos(0)->getRX(),pepsi.GetRenderPos(0)->getRY(),pepsi.GetRenderPos(0)->getRZ());
			modelStack.Scale(pepsi.GetRenderPos(0)->getScaleX(),pepsi.GetRenderPos(0)->getScaleY(),pepsi.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEPSI], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(pepsi.GetRenderPos(1)->getTranslationX()+(j*1.4),pepsi.GetRenderPos(1)->getTranslationY(),pepsi.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(pepsi.GetRenderPos(1)->getRotation(),pepsi.GetRenderPos(1)->getRX(),pepsi.GetRenderPos(1)->getRY(),pepsi.GetRenderPos(1)->getRZ());
			modelStack.Scale(pepsi.GetRenderPos(1)->getScaleX(),pepsi.GetRenderPos(1)->getScaleY(),pepsi.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEPSI], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(pepsi.GetRenderPos(2)->getTranslationX()+(j*1.4),pepsi.GetRenderPos(2)->getTranslationY(),pepsi.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(pepsi.GetRenderPos(2)->getRotation(),pepsi.GetRenderPos(2)->getRX(),pepsi.GetRenderPos(2)->getRY(),pepsi.GetRenderPos(0)->getRZ());
			modelStack.Scale(pepsi.GetRenderPos(2)->getScaleX(),pepsi.GetRenderPos(2)->getScaleY(),pepsi.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEPSI], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side(Pepsi)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(pepsi.GetRenderPos(3)->getTranslationX()+(j*1.4),pepsi.GetRenderPos(3)->getTranslationY(),pepsi.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(pepsi.GetRenderPos(3)->getRotation(),pepsi.GetRenderPos(3)->getRX(),pepsi.GetRenderPos(3)->getRY(),pepsi.GetRenderPos(3)->getRZ());
			modelStack.Scale(pepsi.GetRenderPos(3)->getScaleX(),pepsi.GetRenderPos(3)->getScaleY(),pepsi.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEPSI], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(pepsi.GetRenderPos(4)->getTranslationX()+(j*1.4),pepsi.GetRenderPos(4)->getTranslationY(),pepsi.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(pepsi.GetRenderPos(4)->getRotation(),pepsi.GetRenderPos(4)->getRX(),pepsi.GetRenderPos(4)->getRY(),pepsi.GetRenderPos(4)->getRZ());
			modelStack.Scale(pepsi.GetRenderPos(4)->getScaleX(),pepsi.GetRenderPos(4)->getScaleY(),pepsi.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEPSI], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(pepsi.GetRenderPos(5)->getTranslationX()+(j*1.4),pepsi.GetRenderPos(5)->getTranslationY(),pepsi.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(pepsi.GetRenderPos(5)->getRotation(),pepsi.GetRenderPos(5)->getRX(),pepsi.GetRenderPos(5)->getRY(),pepsi.GetRenderPos(5)->getRZ());
			modelStack.Scale(pepsi.GetRenderPos(5)->getScaleX(),pepsi.GetRenderPos(5)->getScaleY(),pepsi.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEPSI], true);
			modelStack.PopMatrix();
		}
	}

	//Left Side(Coke)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(coke.GetRenderPos(0)->getTranslationX()+(j*1.4),coke.GetRenderPos(0)->getTranslationY(),coke.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(coke.GetRenderPos(0)->getRotation(),coke.GetRenderPos(0)->getRX(),coke.GetRenderPos(0)->getRY(),coke.GetRenderPos(0)->getRZ());
			modelStack.Scale(coke.GetRenderPos(0)->getScaleX(),coke.GetRenderPos(0)->getScaleY(),coke.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_COKE], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(coke.GetRenderPos(1)->getTranslationX()+(j*1.4),coke.GetRenderPos(1)->getTranslationY(),coke.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(coke.GetRenderPos(1)->getRotation(),coke.GetRenderPos(1)->getRX(),coke.GetRenderPos(1)->getRY(),coke.GetRenderPos(1)->getRZ());
			modelStack.Scale(coke.GetRenderPos(1)->getScaleX(),coke.GetRenderPos(1)->getScaleY(),coke.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_COKE], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(coke.GetRenderPos(2)->getTranslationX()+(j*1.4),coke.GetRenderPos(2)->getTranslationY(),coke.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(coke.GetRenderPos(2)->getRotation(),coke.GetRenderPos(2)->getRX(),coke.GetRenderPos(2)->getRY(),coke.GetRenderPos(0)->getRZ());
			modelStack.Scale(coke.GetRenderPos(2)->getScaleX(),coke.GetRenderPos(2)->getScaleY(),coke.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_COKE], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side(Coke)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(coke.GetRenderPos(3)->getTranslationX()+(j*1.4),coke.GetRenderPos(3)->getTranslationY(),coke.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(coke.GetRenderPos(3)->getRotation(),coke.GetRenderPos(3)->getRX(),coke.GetRenderPos(3)->getRY(),coke.GetRenderPos(3)->getRZ());
			modelStack.Scale(coke.GetRenderPos(3)->getScaleX(),coke.GetRenderPos(3)->getScaleY(),coke.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_COKE], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(coke.GetRenderPos(4)->getTranslationX()+(j*1.4),coke.GetRenderPos(4)->getTranslationY(),coke.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(coke.GetRenderPos(4)->getRotation(),coke.GetRenderPos(4)->getRX(),coke.GetRenderPos(4)->getRY(),coke.GetRenderPos(4)->getRZ());
			modelStack.Scale(coke.GetRenderPos(4)->getScaleX(),coke.GetRenderPos(4)->getScaleY(),coke.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_COKE], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(coke.GetRenderPos(5)->getTranslationX()+(j*1.4),coke.GetRenderPos(5)->getTranslationY(),coke.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(coke.GetRenderPos(5)->getRotation(),coke.GetRenderPos(5)->getRX(),coke.GetRenderPos(5)->getRY(),coke.GetRenderPos(5)->getRZ());
			modelStack.Scale(coke.GetRenderPos(5)->getScaleX(),coke.GetRenderPos(5)->getScaleY(),coke.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_COKE], true);
			modelStack.PopMatrix();
		}
	}

	//Left Side(MtDew)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(mtdew.GetRenderPos(0)->getTranslationX()+(j*1.4),mtdew.GetRenderPos(0)->getTranslationY(),mtdew.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(mtdew.GetRenderPos(0)->getRotation(),mtdew.GetRenderPos(0)->getRX(),mtdew.GetRenderPos(0)->getRY(),mtdew.GetRenderPos(0)->getRZ());
			modelStack.Scale(mtdew.GetRenderPos(0)->getScaleX(),mtdew.GetRenderPos(0)->getScaleY(),mtdew.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MTDEW], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(mtdew.GetRenderPos(1)->getTranslationX()+(j*1.4),mtdew.GetRenderPos(1)->getTranslationY(),mtdew.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(mtdew.GetRenderPos(1)->getRotation(),mtdew.GetRenderPos(1)->getRX(),mtdew.GetRenderPos(1)->getRY(),mtdew.GetRenderPos(1)->getRZ());
			modelStack.Scale(mtdew.GetRenderPos(1)->getScaleX(),mtdew.GetRenderPos(1)->getScaleY(),mtdew.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MTDEW], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(mtdew.GetRenderPos(2)->getTranslationX()+(j*1.4),mtdew.GetRenderPos(2)->getTranslationY(),mtdew.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(mtdew.GetRenderPos(2)->getRotation(),mtdew.GetRenderPos(2)->getRX(),mtdew.GetRenderPos(2)->getRY(),mtdew.GetRenderPos(0)->getRZ());
			modelStack.Scale(mtdew.GetRenderPos(2)->getScaleX(),mtdew.GetRenderPos(2)->getScaleY(),mtdew.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MTDEW], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side(MtDew)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(mtdew.GetRenderPos(3)->getTranslationX()+(j*1.4),mtdew.GetRenderPos(3)->getTranslationY(),mtdew.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(mtdew.GetRenderPos(3)->getRotation(),mtdew.GetRenderPos(3)->getRX(),mtdew.GetRenderPos(3)->getRY(),mtdew.GetRenderPos(3)->getRZ());
			modelStack.Scale(mtdew.GetRenderPos(3)->getScaleX(),mtdew.GetRenderPos(3)->getScaleY(),mtdew.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MTDEW], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(mtdew.GetRenderPos(4)->getTranslationX()+(j*1.4),mtdew.GetRenderPos(4)->getTranslationY(),mtdew.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(mtdew.GetRenderPos(4)->getRotation(),mtdew.GetRenderPos(4)->getRX(),mtdew.GetRenderPos(4)->getRY(),mtdew.GetRenderPos(4)->getRZ());
			modelStack.Scale(mtdew.GetRenderPos(4)->getScaleX(),mtdew.GetRenderPos(4)->getScaleY(),mtdew.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MTDEW], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(mtdew.GetRenderPos(5)->getTranslationX()+(j*1.4),mtdew.GetRenderPos(5)->getTranslationY(),mtdew.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(mtdew.GetRenderPos(5)->getRotation(),mtdew.GetRenderPos(5)->getRX(),mtdew.GetRenderPos(5)->getRY(),mtdew.GetRenderPos(5)->getRZ());
			modelStack.Scale(mtdew.GetRenderPos(5)->getScaleX(),mtdew.GetRenderPos(5)->getScaleY(),mtdew.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MTDEW], true);
			modelStack.PopMatrix();
		}
	}

	//Left Side(water)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(watercan.GetRenderPos(0)->getTranslationX()+(j*1.4),watercan.GetRenderPos(0)->getTranslationY(),watercan.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(watercan.GetRenderPos(0)->getRotation(),watercan.GetRenderPos(0)->getRX(),watercan.GetRenderPos(0)->getRY(),watercan.GetRenderPos(0)->getRZ());
			modelStack.Scale(watercan.GetRenderPos(0)->getScaleX(),watercan.GetRenderPos(0)->getScaleY(),watercan.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_WATERCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(watercan.GetRenderPos(1)->getTranslationX()+(j*1.4),watercan.GetRenderPos(1)->getTranslationY(),watercan.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(watercan.GetRenderPos(1)->getRotation(),watercan.GetRenderPos(1)->getRX(),watercan.GetRenderPos(1)->getRY(),watercan.GetRenderPos(1)->getRZ());
			modelStack.Scale(watercan.GetRenderPos(1)->getScaleX(),watercan.GetRenderPos(1)->getScaleY(),watercan.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_WATERCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(watercan.GetRenderPos(2)->getTranslationX()+(j*1.4),watercan.GetRenderPos(2)->getTranslationY(),watercan.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(watercan.GetRenderPos(2)->getRotation(),watercan.GetRenderPos(2)->getRX(),watercan.GetRenderPos(2)->getRY(),watercan.GetRenderPos(0)->getRZ());
			modelStack.Scale(watercan.GetRenderPos(2)->getScaleX(),watercan.GetRenderPos(2)->getScaleY(),watercan.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_WATERCAN], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side(water)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(watercan.GetRenderPos(3)->getTranslationX()+(j*1.4),watercan.GetRenderPos(3)->getTranslationY(),watercan.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(watercan.GetRenderPos(3)->getRotation(),watercan.GetRenderPos(3)->getRX(),watercan.GetRenderPos(3)->getRY(),watercan.GetRenderPos(3)->getRZ());
			modelStack.Scale(watercan.GetRenderPos(3)->getScaleX(),watercan.GetRenderPos(3)->getScaleY(),watercan.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_WATERCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(watercan.GetRenderPos(4)->getTranslationX()+(j*1.4),watercan.GetRenderPos(4)->getTranslationY(),watercan.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(watercan.GetRenderPos(4)->getRotation(),watercan.GetRenderPos(4)->getRX(),watercan.GetRenderPos(4)->getRY(),watercan.GetRenderPos(4)->getRZ());
			modelStack.Scale(watercan.GetRenderPos(4)->getScaleX(),watercan.GetRenderPos(4)->getScaleY(),watercan.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_WATERCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(watercan.GetRenderPos(5)->getTranslationX()+(j*1.4),watercan.GetRenderPos(5)->getTranslationY(),watercan.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(watercan.GetRenderPos(5)->getRotation(),watercan.GetRenderPos(5)->getRX(),watercan.GetRenderPos(5)->getRY(),watercan.GetRenderPos(5)->getRZ());
			modelStack.Scale(watercan.GetRenderPos(5)->getScaleX(),watercan.GetRenderPos(5)->getScaleY(),watercan.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_WATERCAN], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();

	//Shelf 3-9 to 3-12
	//Left Side(Pepsi)
	modelStack.PushMatrix();
	modelStack.Translate(34,0,30);
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(pepsi.GetRenderPos(0)->getTranslationX()+(j*1.4),pepsi.GetRenderPos(0)->getTranslationY(),pepsi.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(pepsi.GetRenderPos(0)->getRotation(),pepsi.GetRenderPos(0)->getRX(),pepsi.GetRenderPos(0)->getRY(),pepsi.GetRenderPos(0)->getRZ());
			modelStack.Scale(pepsi.GetRenderPos(0)->getScaleX(),pepsi.GetRenderPos(0)->getScaleY(),pepsi.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEPSI], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(pepsi.GetRenderPos(1)->getTranslationX()+(j*1.4),pepsi.GetRenderPos(1)->getTranslationY(),pepsi.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(pepsi.GetRenderPos(1)->getRotation(),pepsi.GetRenderPos(1)->getRX(),pepsi.GetRenderPos(1)->getRY(),pepsi.GetRenderPos(1)->getRZ());
			modelStack.Scale(pepsi.GetRenderPos(1)->getScaleX(),pepsi.GetRenderPos(1)->getScaleY(),pepsi.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEPSI], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(pepsi.GetRenderPos(2)->getTranslationX()+(j*1.4),pepsi.GetRenderPos(2)->getTranslationY(),pepsi.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(pepsi.GetRenderPos(2)->getRotation(),pepsi.GetRenderPos(2)->getRX(),pepsi.GetRenderPos(2)->getRY(),pepsi.GetRenderPos(0)->getRZ());
			modelStack.Scale(pepsi.GetRenderPos(2)->getScaleX(),pepsi.GetRenderPos(2)->getScaleY(),pepsi.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEPSI], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side(Pepsi)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(pepsi.GetRenderPos(3)->getTranslationX()+(j*1.4),pepsi.GetRenderPos(3)->getTranslationY(),pepsi.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(pepsi.GetRenderPos(3)->getRotation(),pepsi.GetRenderPos(3)->getRX(),pepsi.GetRenderPos(3)->getRY(),pepsi.GetRenderPos(3)->getRZ());
			modelStack.Scale(pepsi.GetRenderPos(3)->getScaleX(),pepsi.GetRenderPos(3)->getScaleY(),pepsi.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEPSI], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(pepsi.GetRenderPos(4)->getTranslationX()+(j*1.4),pepsi.GetRenderPos(4)->getTranslationY(),pepsi.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(pepsi.GetRenderPos(4)->getRotation(),pepsi.GetRenderPos(4)->getRX(),pepsi.GetRenderPos(4)->getRY(),pepsi.GetRenderPos(4)->getRZ());
			modelStack.Scale(pepsi.GetRenderPos(4)->getScaleX(),pepsi.GetRenderPos(4)->getScaleY(),pepsi.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEPSI], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(pepsi.GetRenderPos(5)->getTranslationX()+(j*1.4),pepsi.GetRenderPos(5)->getTranslationY(),pepsi.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(pepsi.GetRenderPos(5)->getRotation(),pepsi.GetRenderPos(5)->getRX(),pepsi.GetRenderPos(5)->getRY(),pepsi.GetRenderPos(5)->getRZ());
			modelStack.Scale(pepsi.GetRenderPos(5)->getScaleX(),pepsi.GetRenderPos(5)->getScaleY(),pepsi.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PEPSI], true);
			modelStack.PopMatrix();
		}
	}

	//Left Side(Coke)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(coke.GetRenderPos(0)->getTranslationX()+(j*1.4),coke.GetRenderPos(0)->getTranslationY(),coke.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(coke.GetRenderPos(0)->getRotation(),coke.GetRenderPos(0)->getRX(),coke.GetRenderPos(0)->getRY(),coke.GetRenderPos(0)->getRZ());
			modelStack.Scale(coke.GetRenderPos(0)->getScaleX(),coke.GetRenderPos(0)->getScaleY(),coke.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_COKE], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(coke.GetRenderPos(1)->getTranslationX()+(j*1.4),coke.GetRenderPos(1)->getTranslationY(),coke.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(coke.GetRenderPos(1)->getRotation(),coke.GetRenderPos(1)->getRX(),coke.GetRenderPos(1)->getRY(),coke.GetRenderPos(1)->getRZ());
			modelStack.Scale(coke.GetRenderPos(1)->getScaleX(),coke.GetRenderPos(1)->getScaleY(),coke.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_COKE], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(coke.GetRenderPos(2)->getTranslationX()+(j*1.4),coke.GetRenderPos(2)->getTranslationY(),coke.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(coke.GetRenderPos(2)->getRotation(),coke.GetRenderPos(2)->getRX(),coke.GetRenderPos(2)->getRY(),coke.GetRenderPos(0)->getRZ());
			modelStack.Scale(coke.GetRenderPos(2)->getScaleX(),coke.GetRenderPos(2)->getScaleY(),coke.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_COKE], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side(Coke)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(coke.GetRenderPos(3)->getTranslationX()+(j*1.4),coke.GetRenderPos(3)->getTranslationY(),coke.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(coke.GetRenderPos(3)->getRotation(),coke.GetRenderPos(3)->getRX(),coke.GetRenderPos(3)->getRY(),coke.GetRenderPos(3)->getRZ());
			modelStack.Scale(coke.GetRenderPos(3)->getScaleX(),coke.GetRenderPos(3)->getScaleY(),coke.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_COKE], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(coke.GetRenderPos(4)->getTranslationX()+(j*1.4),coke.GetRenderPos(4)->getTranslationY(),coke.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(coke.GetRenderPos(4)->getRotation(),coke.GetRenderPos(4)->getRX(),coke.GetRenderPos(4)->getRY(),coke.GetRenderPos(4)->getRZ());
			modelStack.Scale(coke.GetRenderPos(4)->getScaleX(),coke.GetRenderPos(4)->getScaleY(),coke.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_COKE], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(coke.GetRenderPos(5)->getTranslationX()+(j*1.4),coke.GetRenderPos(5)->getTranslationY(),coke.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(coke.GetRenderPos(5)->getRotation(),coke.GetRenderPos(5)->getRX(),coke.GetRenderPos(5)->getRY(),coke.GetRenderPos(5)->getRZ());
			modelStack.Scale(coke.GetRenderPos(5)->getScaleX(),coke.GetRenderPos(5)->getScaleY(),coke.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_COKE], true);
			modelStack.PopMatrix();
		}
	}

	//Left Side(MtDew)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(mtdew.GetRenderPos(0)->getTranslationX()+(j*1.4),mtdew.GetRenderPos(0)->getTranslationY(),mtdew.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(mtdew.GetRenderPos(0)->getRotation(),mtdew.GetRenderPos(0)->getRX(),mtdew.GetRenderPos(0)->getRY(),mtdew.GetRenderPos(0)->getRZ());
			modelStack.Scale(mtdew.GetRenderPos(0)->getScaleX(),mtdew.GetRenderPos(0)->getScaleY(),mtdew.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MTDEW], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(mtdew.GetRenderPos(1)->getTranslationX()+(j*1.4),mtdew.GetRenderPos(1)->getTranslationY(),mtdew.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(mtdew.GetRenderPos(1)->getRotation(),mtdew.GetRenderPos(1)->getRX(),mtdew.GetRenderPos(1)->getRY(),mtdew.GetRenderPos(1)->getRZ());
			modelStack.Scale(mtdew.GetRenderPos(1)->getScaleX(),mtdew.GetRenderPos(1)->getScaleY(),mtdew.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MTDEW], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(mtdew.GetRenderPos(2)->getTranslationX()+(j*1.4),mtdew.GetRenderPos(2)->getTranslationY(),mtdew.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(mtdew.GetRenderPos(2)->getRotation(),mtdew.GetRenderPos(2)->getRX(),mtdew.GetRenderPos(2)->getRY(),mtdew.GetRenderPos(0)->getRZ());
			modelStack.Scale(mtdew.GetRenderPos(2)->getScaleX(),mtdew.GetRenderPos(2)->getScaleY(),mtdew.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MTDEW], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side(MtDew)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(mtdew.GetRenderPos(3)->getTranslationX()+(j*1.4),mtdew.GetRenderPos(3)->getTranslationY(),mtdew.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(mtdew.GetRenderPos(3)->getRotation(),mtdew.GetRenderPos(3)->getRX(),mtdew.GetRenderPos(3)->getRY(),mtdew.GetRenderPos(3)->getRZ());
			modelStack.Scale(mtdew.GetRenderPos(3)->getScaleX(),mtdew.GetRenderPos(3)->getScaleY(),mtdew.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MTDEW], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(mtdew.GetRenderPos(4)->getTranslationX()+(j*1.4),mtdew.GetRenderPos(4)->getTranslationY(),mtdew.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(mtdew.GetRenderPos(4)->getRotation(),mtdew.GetRenderPos(4)->getRX(),mtdew.GetRenderPos(4)->getRY(),mtdew.GetRenderPos(4)->getRZ());
			modelStack.Scale(mtdew.GetRenderPos(4)->getScaleX(),mtdew.GetRenderPos(4)->getScaleY(),mtdew.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MTDEW], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(mtdew.GetRenderPos(5)->getTranslationX()+(j*1.4),mtdew.GetRenderPos(5)->getTranslationY(),mtdew.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(mtdew.GetRenderPos(5)->getRotation(),mtdew.GetRenderPos(5)->getRX(),mtdew.GetRenderPos(5)->getRY(),mtdew.GetRenderPos(5)->getRZ());
			modelStack.Scale(mtdew.GetRenderPos(5)->getScaleX(),mtdew.GetRenderPos(5)->getScaleY(),mtdew.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MTDEW], true);
			modelStack.PopMatrix();
		}
	}

	//Left Side(water)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(watercan.GetRenderPos(0)->getTranslationX()+(j*1.4),watercan.GetRenderPos(0)->getTranslationY(),watercan.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(watercan.GetRenderPos(0)->getRotation(),watercan.GetRenderPos(0)->getRX(),watercan.GetRenderPos(0)->getRY(),watercan.GetRenderPos(0)->getRZ());
			modelStack.Scale(watercan.GetRenderPos(0)->getScaleX(),watercan.GetRenderPos(0)->getScaleY(),watercan.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_WATERCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(watercan.GetRenderPos(1)->getTranslationX()+(j*1.4),watercan.GetRenderPos(1)->getTranslationY(),watercan.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(watercan.GetRenderPos(1)->getRotation(),watercan.GetRenderPos(1)->getRX(),watercan.GetRenderPos(1)->getRY(),watercan.GetRenderPos(1)->getRZ());
			modelStack.Scale(watercan.GetRenderPos(1)->getScaleX(),watercan.GetRenderPos(1)->getScaleY(),watercan.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_WATERCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(watercan.GetRenderPos(2)->getTranslationX()+(j*1.4),watercan.GetRenderPos(2)->getTranslationY(),watercan.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(watercan.GetRenderPos(2)->getRotation(),watercan.GetRenderPos(2)->getRX(),watercan.GetRenderPos(2)->getRY(),watercan.GetRenderPos(0)->getRZ());
			modelStack.Scale(watercan.GetRenderPos(2)->getScaleX(),watercan.GetRenderPos(2)->getScaleY(),watercan.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_WATERCAN], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side(water)
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(watercan.GetRenderPos(3)->getTranslationX()+(j*1.4),watercan.GetRenderPos(3)->getTranslationY(),watercan.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(watercan.GetRenderPos(3)->getRotation(),watercan.GetRenderPos(3)->getRX(),watercan.GetRenderPos(3)->getRY(),watercan.GetRenderPos(3)->getRZ());
			modelStack.Scale(watercan.GetRenderPos(3)->getScaleX(),watercan.GetRenderPos(3)->getScaleY(),watercan.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_WATERCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(watercan.GetRenderPos(4)->getTranslationX()+(j*1.4),watercan.GetRenderPos(4)->getTranslationY(),watercan.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(watercan.GetRenderPos(4)->getRotation(),watercan.GetRenderPos(4)->getRX(),watercan.GetRenderPos(4)->getRY(),watercan.GetRenderPos(4)->getRZ());
			modelStack.Scale(watercan.GetRenderPos(4)->getScaleX(),watercan.GetRenderPos(4)->getScaleY(),watercan.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_WATERCAN], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(watercan.GetRenderPos(5)->getTranslationX()+(j*1.4),watercan.GetRenderPos(5)->getTranslationY(),watercan.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(watercan.GetRenderPos(5)->getRotation(),watercan.GetRenderPos(5)->getRX(),watercan.GetRenderPos(5)->getRY(),watercan.GetRenderPos(5)->getRZ());
			modelStack.Scale(watercan.GetRenderPos(5)->getScaleX(),watercan.GetRenderPos(5)->getScaleY(),watercan.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_WATERCAN], true);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
}

void SP2::RenderChips()
{
	modelStack.PushMatrix();
	//Shelf1-9
	//Left Side
	for(int k = 0; k < 3; ++k)
	{
			//BTM
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(0)->getTranslationX()+(j*2),chips.GetRenderPos(0)->getTranslationY()+(k*0.3),chips.GetRenderPos(0)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(0)->getRotation(),chips.GetRenderPos(0)->getRX(),chips.GetRenderPos(0)->getRY(),chips.GetRenderPos(0)->getRZ());
				modelStack.Scale(chips.GetRenderPos(0)->getScaleX(),chips.GetRenderPos(0)->getScaleY(),chips.GetRenderPos(0)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
			//MIDDLE
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(1)->getTranslationX()+(j*2),chips.GetRenderPos(1)->getTranslationY()+(k*0.3),chips.GetRenderPos(1)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(1)->getRotation(),chips.GetRenderPos(1)->getRX(),chips.GetRenderPos(1)->getRY(),chips.GetRenderPos(1)->getRZ());
				modelStack.Scale(chips.GetRenderPos(1)->getScaleX(),chips.GetRenderPos(1)->getScaleY(),chips.GetRenderPos(1)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
			//TOP
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(2)->getTranslationX()+(j*2),chips.GetRenderPos(2)->getTranslationY()+(k*0.3),chips.GetRenderPos(2)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(2)->getRotation(),chips.GetRenderPos(2)->getRX(),chips.GetRenderPos(2)->getRY(),chips.GetRenderPos(0)->getRZ());
				modelStack.Scale(chips.GetRenderPos(2)->getScaleX(),chips.GetRenderPos(2)->getScaleY(),chips.GetRenderPos(2)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}

	//RIGHT Side
	for(int k = 0; k < 3; ++k)
	{
		
			//BTM
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(3)->getTranslationX()+(j*2),chips.GetRenderPos(3)->getTranslationY()+(k*0.3),chips.GetRenderPos(3)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(3)->getRotation(),chips.GetRenderPos(3)->getRX(),chips.GetRenderPos(3)->getRY(),chips.GetRenderPos(3)->getRZ());
				modelStack.Scale(chips.GetRenderPos(3)->getScaleX(),chips.GetRenderPos(3)->getScaleY(),chips.GetRenderPos(3)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
		
			//MIDDLE
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(4)->getTranslationX()+(j*2),chips.GetRenderPos(4)->getTranslationY()+(k*0.3),chips.GetRenderPos(4)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(4)->getRotation(),chips.GetRenderPos(4)->getRX(),chips.GetRenderPos(4)->getRY(),chips.GetRenderPos(4)->getRZ());
				modelStack.Scale(chips.GetRenderPos(4)->getScaleX(),chips.GetRenderPos(4)->getScaleY(),chips.GetRenderPos(4)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
			//TOP
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(5)->getTranslationX()+(j*2),chips.GetRenderPos(5)->getTranslationY()+(k*0.3),chips.GetRenderPos(5)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(5)->getRotation(),chips.GetRenderPos(5)->getRX(),chips.GetRenderPos(5)->getRY(),chips.GetRenderPos(5)->getRZ());
				modelStack.Scale(chips.GetRenderPos(5)->getScaleX(),chips.GetRenderPos(5)->getScaleY(),chips.GetRenderPos(5)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//Shelf2-3
	//Left Side
	modelStack.Translate(-56,0,15);
	for(int k = 0; k < 3; ++k)
	{
			//BTM
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(0)->getTranslationX()+(j*2),chips.GetRenderPos(0)->getTranslationY()+(k*0.3),chips.GetRenderPos(0)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(0)->getRotation(),chips.GetRenderPos(0)->getRX(),chips.GetRenderPos(0)->getRY(),chips.GetRenderPos(0)->getRZ());
				modelStack.Scale(chips.GetRenderPos(0)->getScaleX(),chips.GetRenderPos(0)->getScaleY(),chips.GetRenderPos(0)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
			//MIDDLE
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(1)->getTranslationX()+(j*2),chips.GetRenderPos(1)->getTranslationY()+(k*0.3),chips.GetRenderPos(1)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(1)->getRotation(),chips.GetRenderPos(1)->getRX(),chips.GetRenderPos(1)->getRY(),chips.GetRenderPos(1)->getRZ());
				modelStack.Scale(chips.GetRenderPos(1)->getScaleX(),chips.GetRenderPos(1)->getScaleY(),chips.GetRenderPos(1)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
			//TOP
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(2)->getTranslationX()+(j*2),chips.GetRenderPos(2)->getTranslationY()+(k*0.3),chips.GetRenderPos(2)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(2)->getRotation(),chips.GetRenderPos(2)->getRX(),chips.GetRenderPos(2)->getRY(),chips.GetRenderPos(0)->getRZ());
				modelStack.Scale(chips.GetRenderPos(2)->getScaleX(),chips.GetRenderPos(2)->getScaleY(),chips.GetRenderPos(2)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}

	//RIGHT Side
	for(int k = 0; k < 3; ++k)
	{
		
			//BTM
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(3)->getTranslationX()+(j*2),chips.GetRenderPos(3)->getTranslationY()+(k*0.3),chips.GetRenderPos(3)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(3)->getRotation(),chips.GetRenderPos(3)->getRX(),chips.GetRenderPos(3)->getRY(),chips.GetRenderPos(3)->getRZ());
				modelStack.Scale(chips.GetRenderPos(3)->getScaleX(),chips.GetRenderPos(3)->getScaleY(),chips.GetRenderPos(3)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
		
			//MIDDLE
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(4)->getTranslationX()+(j*2),chips.GetRenderPos(4)->getTranslationY()+(k*0.3),chips.GetRenderPos(4)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(4)->getRotation(),chips.GetRenderPos(4)->getRX(),chips.GetRenderPos(4)->getRY(),chips.GetRenderPos(4)->getRZ());
				modelStack.Scale(chips.GetRenderPos(4)->getScaleX(),chips.GetRenderPos(4)->getScaleY(),chips.GetRenderPos(4)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
			//TOP
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(5)->getTranslationX()+(j*2),chips.GetRenderPos(5)->getTranslationY()+(k*0.3),chips.GetRenderPos(5)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(5)->getRotation(),chips.GetRenderPos(5)->getRX(),chips.GetRenderPos(5)->getRY(),chips.GetRenderPos(5)->getRZ());
				modelStack.Scale(chips.GetRenderPos(5)->getScaleX(),chips.GetRenderPos(5)->getScaleY(),chips.GetRenderPos(5)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//Shelf2-9
	//Left Side
	modelStack.Translate(18,0,15);
	for(int k = 0; k < 3; ++k)
	{
			//BTM
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(0)->getTranslationX()+(j*2),chips.GetRenderPos(0)->getTranslationY()+(k*0.3),chips.GetRenderPos(0)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(0)->getRotation(),chips.GetRenderPos(0)->getRX(),chips.GetRenderPos(0)->getRY(),chips.GetRenderPos(0)->getRZ());
				modelStack.Scale(chips.GetRenderPos(0)->getScaleX(),chips.GetRenderPos(0)->getScaleY(),chips.GetRenderPos(0)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
			//MIDDLE
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(1)->getTranslationX()+(j*2),chips.GetRenderPos(1)->getTranslationY()+(k*0.3),chips.GetRenderPos(1)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(1)->getRotation(),chips.GetRenderPos(1)->getRX(),chips.GetRenderPos(1)->getRY(),chips.GetRenderPos(1)->getRZ());
				modelStack.Scale(chips.GetRenderPos(1)->getScaleX(),chips.GetRenderPos(1)->getScaleY(),chips.GetRenderPos(1)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
			//TOP
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(2)->getTranslationX()+(j*2),chips.GetRenderPos(2)->getTranslationY()+(k*0.3),chips.GetRenderPos(2)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(2)->getRotation(),chips.GetRenderPos(2)->getRX(),chips.GetRenderPos(2)->getRY(),chips.GetRenderPos(0)->getRZ());
				modelStack.Scale(chips.GetRenderPos(2)->getScaleX(),chips.GetRenderPos(2)->getScaleY(),chips.GetRenderPos(2)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}

	//RIGHT Side
	for(int k = 0; k < 3; ++k)
	{
		
			//BTM
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(3)->getTranslationX()+(j*2),chips.GetRenderPos(3)->getTranslationY()+(k*0.3),chips.GetRenderPos(3)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(3)->getRotation(),chips.GetRenderPos(3)->getRX(),chips.GetRenderPos(3)->getRY(),chips.GetRenderPos(3)->getRZ());
				modelStack.Scale(chips.GetRenderPos(3)->getScaleX(),chips.GetRenderPos(3)->getScaleY(),chips.GetRenderPos(3)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
		
			//MIDDLE
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(4)->getTranslationX()+(j*2),chips.GetRenderPos(4)->getTranslationY()+(k*0.3),chips.GetRenderPos(4)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(4)->getRotation(),chips.GetRenderPos(4)->getRX(),chips.GetRenderPos(4)->getRY(),chips.GetRenderPos(4)->getRZ());
				modelStack.Scale(chips.GetRenderPos(4)->getScaleX(),chips.GetRenderPos(4)->getScaleY(),chips.GetRenderPos(4)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
			//TOP
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chips.GetRenderPos(5)->getTranslationX()+(j*2),chips.GetRenderPos(5)->getTranslationY()+(k*0.3),chips.GetRenderPos(5)->getTranslationZ());
				modelStack.Rotate(chips.GetRenderPos(5)->getRotation(),chips.GetRenderPos(5)->getRX(),chips.GetRenderPos(5)->getRY(),chips.GetRenderPos(5)->getRZ());
				modelStack.Scale(chips.GetRenderPos(5)->getScaleX(),chips.GetRenderPos(5)->getScaleY(),chips.GetRenderPos(5)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_LAYSCHIPS], true);
				modelStack.PopMatrix();
			}
	}
	modelStack.PopMatrix();
}

void SP2::RenderChocolate()
{
	modelStack.PushMatrix();
	//Shelf2-1
	//Left Side
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//BTM
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(0)->getTranslationX()+(j*1),chocolate.GetRenderPos(0)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(0)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(0)->getRotation(),chocolate.GetRenderPos(0)->getRX(),chocolate.GetRenderPos(0)->getRY(),chocolate.GetRenderPos(0)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(0)->getScaleX(),chocolate.GetRenderPos(0)->getScaleY(),chocolate.GetRenderPos(0)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//MIDDLE
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(1)->getTranslationX()+(j*1),chocolate.GetRenderPos(1)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(1)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(1)->getRotation(),chocolate.GetRenderPos(1)->getRX(),chocolate.GetRenderPos(1)->getRY(),chocolate.GetRenderPos(1)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(1)->getScaleX(),chocolate.GetRenderPos(1)->getScaleY(),chocolate.GetRenderPos(1)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//TOP
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(2)->getTranslationX()+(j*1),chocolate.GetRenderPos(2)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(2)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(2)->getRotation(),chocolate.GetRenderPos(2)->getRX(),chocolate.GetRenderPos(2)->getRY(),chocolate.GetRenderPos(0)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(2)->getScaleX(),chocolate.GetRenderPos(2)->getScaleY(),chocolate.GetRenderPos(2)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}

	//RIGHT Side
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//BTM
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(3)->getTranslationX()+(j*1),chocolate.GetRenderPos(3)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(3)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(3)->getRotation(),chocolate.GetRenderPos(3)->getRX(),chocolate.GetRenderPos(3)->getRY(),chocolate.GetRenderPos(3)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(3)->getScaleX(),chocolate.GetRenderPos(3)->getScaleY(),chocolate.GetRenderPos(3)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//MIDDLE
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(4)->getTranslationX()+(j*1),chocolate.GetRenderPos(4)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(4)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(4)->getRotation(),chocolate.GetRenderPos(4)->getRX(),chocolate.GetRenderPos(4)->getRY(),chocolate.GetRenderPos(4)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(4)->getScaleX(),chocolate.GetRenderPos(4)->getScaleY(),chocolate.GetRenderPos(4)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//TOP
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(5)->getTranslationX()+(j*1),chocolate.GetRenderPos(5)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(5)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(5)->getRotation(),chocolate.GetRenderPos(5)->getRX(),chocolate.GetRenderPos(5)->getRY(),chocolate.GetRenderPos(5)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(5)->getScaleX(),chocolate.GetRenderPos(5)->getScaleY(),chocolate.GetRenderPos(5)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//Shelf2-4
	//Left Side
	modelStack.Translate(18.2,0.7,0.6);
	for(int k = 0; k < 3; ++k)
	{

			//BTM
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(0)->getTranslationX()+(j*1.8),chocolate.GetRenderPos(0)->getTranslationY(),chocolate.GetRenderPos(0)->getTranslationZ()+(k*0.3));
				modelStack.Rotate(chocolate.GetRenderPos(0)->getRotation(),chocolate.GetRenderPos(0)->getRX(),chocolate.GetRenderPos(0)->getRY(),chocolate.GetRenderPos(0)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(0)->getScaleX(),chocolate.GetRenderPos(0)->getScaleY(),chocolate.GetRenderPos(0)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_ROCHER], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
		
			//MIDDLE
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(1)->getTranslationX()+(j*1.8),chocolate.GetRenderPos(1)->getTranslationY(),chocolate.GetRenderPos(1)->getTranslationZ()+(k*0.3));
				modelStack.Rotate(chocolate.GetRenderPos(1)->getRotation(),chocolate.GetRenderPos(1)->getRX(),chocolate.GetRenderPos(1)->getRY(),chocolate.GetRenderPos(1)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(1)->getScaleX(),chocolate.GetRenderPos(1)->getScaleY(),chocolate.GetRenderPos(1)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_ROCHER], true);
				modelStack.PopMatrix();
			}

	}
	for(int k = 0; k < 3; ++k)
	{
		
			//TOP
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(2)->getTranslationX()+(j*1.8),chocolate.GetRenderPos(2)->getTranslationY(),chocolate.GetRenderPos(2)->getTranslationZ()+(k*0.3));
				modelStack.Rotate(chocolate.GetRenderPos(2)->getRotation(),chocolate.GetRenderPos(2)->getRX(),chocolate.GetRenderPos(2)->getRY(),chocolate.GetRenderPos(0)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(2)->getScaleX(),chocolate.GetRenderPos(2)->getScaleY(),chocolate.GetRenderPos(2)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_ROCHER], true);
				modelStack.PopMatrix();
			}
		
	}

	//RIGHT Side
	for(int k = 0; k < 3; ++k)
	{
		
			//BTM
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(3)->getTranslationX()+(j*1.8),chocolate.GetRenderPos(3)->getTranslationY(),chocolate.GetRenderPos(3)->getTranslationZ()+(k*0.3));
				modelStack.Rotate(chocolate.GetRenderPos(3)->getRotation(),chocolate.GetRenderPos(3)->getRX(),chocolate.GetRenderPos(3)->getRY(),chocolate.GetRenderPos(3)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(3)->getScaleX(),chocolate.GetRenderPos(3)->getScaleY(),chocolate.GetRenderPos(3)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_ROCHER], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
			//MIDDLE
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(4)->getTranslationX()+(j*1.8),chocolate.GetRenderPos(4)->getTranslationY(),chocolate.GetRenderPos(4)->getTranslationZ()+(k*0.3));
				modelStack.Rotate(chocolate.GetRenderPos(4)->getRotation(),chocolate.GetRenderPos(4)->getRX(),chocolate.GetRenderPos(4)->getRY(),chocolate.GetRenderPos(4)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(4)->getScaleX(),chocolate.GetRenderPos(4)->getScaleY(),chocolate.GetRenderPos(4)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_ROCHER], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
			//TOP
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(5)->getTranslationX()+(j*1.8),chocolate.GetRenderPos(5)->getTranslationY(),chocolate.GetRenderPos(5)->getTranslationZ()+(k*0.3));
				modelStack.Rotate(chocolate.GetRenderPos(5)->getRotation(),chocolate.GetRenderPos(5)->getRX(),chocolate.GetRenderPos(5)->getRY(),chocolate.GetRenderPos(5)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(5)->getScaleX(),chocolate.GetRenderPos(5)->getScaleY(),chocolate.GetRenderPos(5)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_ROCHER], true);
				modelStack.PopMatrix();
			}
	}
	modelStack.PopMatrix();

	//Shelf2-9
	//Left Side
	modelStack.PushMatrix();
	modelStack.Translate(68,0,0);
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//BTM
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(0)->getTranslationX()+(j*1),chocolate.GetRenderPos(0)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(0)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(0)->getRotation(),chocolate.GetRenderPos(0)->getRX(),chocolate.GetRenderPos(0)->getRY(),chocolate.GetRenderPos(0)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(0)->getScaleX(),chocolate.GetRenderPos(0)->getScaleY(),chocolate.GetRenderPos(0)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//MIDDLE
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(1)->getTranslationX()+(j*1),chocolate.GetRenderPos(1)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(1)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(1)->getRotation(),chocolate.GetRenderPos(1)->getRX(),chocolate.GetRenderPos(1)->getRY(),chocolate.GetRenderPos(1)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(1)->getScaleX(),chocolate.GetRenderPos(1)->getScaleY(),chocolate.GetRenderPos(1)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//TOP
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(2)->getTranslationX()+(j*1),chocolate.GetRenderPos(2)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(2)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(2)->getRotation(),chocolate.GetRenderPos(2)->getRX(),chocolate.GetRenderPos(2)->getRY(),chocolate.GetRenderPos(0)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(2)->getScaleX(),chocolate.GetRenderPos(2)->getScaleY(),chocolate.GetRenderPos(2)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}

	//RIGHT Side
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//BTM
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(3)->getTranslationX()+(j*1),chocolate.GetRenderPos(3)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(3)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(3)->getRotation(),chocolate.GetRenderPos(3)->getRX(),chocolate.GetRenderPos(3)->getRY(),chocolate.GetRenderPos(3)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(3)->getScaleX(),chocolate.GetRenderPos(3)->getScaleY(),chocolate.GetRenderPos(3)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//MIDDLE
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(4)->getTranslationX()+(j*1),chocolate.GetRenderPos(4)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(4)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(4)->getRotation(),chocolate.GetRenderPos(4)->getRX(),chocolate.GetRenderPos(4)->getRY(),chocolate.GetRenderPos(4)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(4)->getScaleX(),chocolate.GetRenderPos(4)->getScaleY(),chocolate.GetRenderPos(4)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//TOP
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(5)->getTranslationX()+(j*1),chocolate.GetRenderPos(5)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(5)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(5)->getRotation(),chocolate.GetRenderPos(5)->getRX(),chocolate.GetRenderPos(5)->getRY(),chocolate.GetRenderPos(5)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(5)->getScaleX(),chocolate.GetRenderPos(5)->getScaleY(),chocolate.GetRenderPos(5)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	//Shelf2-11
	//Left Side
	modelStack.Translate(80.2,0.7,0.6);
	for(int k = 0; k < 3; ++k)
	{

			//BTM
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(0)->getTranslationX()+(j*1.8),chocolate.GetRenderPos(0)->getTranslationY(),chocolate.GetRenderPos(0)->getTranslationZ()+(k*0.3));
				modelStack.Rotate(chocolate.GetRenderPos(0)->getRotation(),chocolate.GetRenderPos(0)->getRX(),chocolate.GetRenderPos(0)->getRY(),chocolate.GetRenderPos(0)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(0)->getScaleX(),chocolate.GetRenderPos(0)->getScaleY(),chocolate.GetRenderPos(0)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_ROCHER], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
		
			//MIDDLE
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(1)->getTranslationX()+(j*1.8),chocolate.GetRenderPos(1)->getTranslationY(),chocolate.GetRenderPos(1)->getTranslationZ()+(k*0.3));
				modelStack.Rotate(chocolate.GetRenderPos(1)->getRotation(),chocolate.GetRenderPos(1)->getRX(),chocolate.GetRenderPos(1)->getRY(),chocolate.GetRenderPos(1)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(1)->getScaleX(),chocolate.GetRenderPos(1)->getScaleY(),chocolate.GetRenderPos(1)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_ROCHER], true);
				modelStack.PopMatrix();
			}

	}
	for(int k = 0; k < 3; ++k)
	{
		
			//TOP
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(2)->getTranslationX()+(j*1.8),chocolate.GetRenderPos(2)->getTranslationY(),chocolate.GetRenderPos(2)->getTranslationZ()+(k*0.3));
				modelStack.Rotate(chocolate.GetRenderPos(2)->getRotation(),chocolate.GetRenderPos(2)->getRX(),chocolate.GetRenderPos(2)->getRY(),chocolate.GetRenderPos(0)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(2)->getScaleX(),chocolate.GetRenderPos(2)->getScaleY(),chocolate.GetRenderPos(2)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_ROCHER], true);
				modelStack.PopMatrix();
			}
		
	}

	//RIGHT Side
	for(int k = 0; k < 3; ++k)
	{
		
			//BTM
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(3)->getTranslationX()+(j*1.8),chocolate.GetRenderPos(3)->getTranslationY(),chocolate.GetRenderPos(3)->getTranslationZ()+(k*0.3));
				modelStack.Rotate(chocolate.GetRenderPos(3)->getRotation(),chocolate.GetRenderPos(3)->getRX(),chocolate.GetRenderPos(3)->getRY(),chocolate.GetRenderPos(3)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(3)->getScaleX(),chocolate.GetRenderPos(3)->getScaleY(),chocolate.GetRenderPos(3)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_ROCHER], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
			//MIDDLE
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(4)->getTranslationX()+(j*1.8),chocolate.GetRenderPos(4)->getTranslationY(),chocolate.GetRenderPos(4)->getTranslationZ()+(k*0.3));
				modelStack.Rotate(chocolate.GetRenderPos(4)->getRotation(),chocolate.GetRenderPos(4)->getRX(),chocolate.GetRenderPos(4)->getRY(),chocolate.GetRenderPos(4)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(4)->getScaleX(),chocolate.GetRenderPos(4)->getScaleY(),chocolate.GetRenderPos(4)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_ROCHER], true);
				modelStack.PopMatrix();
			}
	}
	for(int k = 0; k < 3; ++k)
	{
			//TOP
			for(int j = 0; j < 3; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(5)->getTranslationX()+(j*1.8),chocolate.GetRenderPos(5)->getTranslationY(),chocolate.GetRenderPos(5)->getTranslationZ()+(k*0.3));
				modelStack.Rotate(chocolate.GetRenderPos(5)->getRotation(),chocolate.GetRenderPos(5)->getRX(),chocolate.GetRenderPos(5)->getRY(),chocolate.GetRenderPos(5)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(5)->getScaleX(),chocolate.GetRenderPos(5)->getScaleY(),chocolate.GetRenderPos(5)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_ROCHER], true);
				modelStack.PopMatrix();
			}
	}
	modelStack.PopMatrix();

	//Shelf1-12
	//Left Side
	modelStack.PushMatrix();
	modelStack.Translate(86,0,-15);
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//BTM
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(0)->getTranslationX()+(j*1),chocolate.GetRenderPos(0)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(0)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(0)->getRotation(),chocolate.GetRenderPos(0)->getRX(),chocolate.GetRenderPos(0)->getRY(),chocolate.GetRenderPos(0)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(0)->getScaleX(),chocolate.GetRenderPos(0)->getScaleY(),chocolate.GetRenderPos(0)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//MIDDLE
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(1)->getTranslationX()+(j*1),chocolate.GetRenderPos(1)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(1)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(1)->getRotation(),chocolate.GetRenderPos(1)->getRX(),chocolate.GetRenderPos(1)->getRY(),chocolate.GetRenderPos(1)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(1)->getScaleX(),chocolate.GetRenderPos(1)->getScaleY(),chocolate.GetRenderPos(1)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//TOP
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(2)->getTranslationX()+(j*1),chocolate.GetRenderPos(2)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(2)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(2)->getRotation(),chocolate.GetRenderPos(2)->getRX(),chocolate.GetRenderPos(2)->getRY(),chocolate.GetRenderPos(0)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(2)->getScaleX(),chocolate.GetRenderPos(2)->getScaleY(),chocolate.GetRenderPos(2)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}

	//RIGHT Side
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//BTM
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(3)->getTranslationX()+(j*1),chocolate.GetRenderPos(3)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(3)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(3)->getRotation(),chocolate.GetRenderPos(3)->getRX(),chocolate.GetRenderPos(3)->getRY(),chocolate.GetRenderPos(3)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(3)->getScaleX(),chocolate.GetRenderPos(3)->getScaleY(),chocolate.GetRenderPos(3)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//MIDDLE
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(4)->getTranslationX()+(j*1),chocolate.GetRenderPos(4)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(4)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(4)->getRotation(),chocolate.GetRenderPos(4)->getRX(),chocolate.GetRenderPos(4)->getRY(),chocolate.GetRenderPos(4)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(4)->getScaleX(),chocolate.GetRenderPos(4)->getScaleY(),chocolate.GetRenderPos(4)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}
	for(int k = 0; k < 4; ++k)
	{
		for(int i = 1; i <= 2; ++ i )
		{
			//TOP
			for(int j = 0; j < 5; ++ j )
			{
				modelStack.PushMatrix();
				modelStack.Translate(chocolate.GetRenderPos(5)->getTranslationX()+(j*1),chocolate.GetRenderPos(5)->getTranslationY()+(k*0.2),chocolate.GetRenderPos(5)->getTranslationZ()+(i*0.6));
				modelStack.Rotate(chocolate.GetRenderPos(5)->getRotation(),chocolate.GetRenderPos(5)->getRX(),chocolate.GetRenderPos(5)->getRY(),chocolate.GetRenderPos(5)->getRZ());
				modelStack.Scale(chocolate.GetRenderPos(5)->getScaleX(),chocolate.GetRenderPos(5)->getScaleY(),chocolate.GetRenderPos(5)->getScaleZ());
				RenderMesh(meshList[GEO_MODEL_HERSHEY], true);
				modelStack.PopMatrix();
			}
		}
	}
	modelStack.PopMatrix();
}

void SP2::RenderCereal()
{
	modelStack.PushMatrix();
	//Shelf1-1
	//Left Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(0)->getTranslationX()+(j*1.4),cereal.GetRenderPos(0)->getTranslationY(),cereal.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(0)->getRotation(),cereal.GetRenderPos(0)->getRX(),cereal.GetRenderPos(0)->getRY(),cereal.GetRenderPos(0)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(0)->getScaleX(),cereal.GetRenderPos(0)->getScaleY(),cereal.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_1], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(1)->getTranslationX()+(j*1.4),cereal.GetRenderPos(1)->getTranslationY(),cereal.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(1)->getRotation(),cereal.GetRenderPos(1)->getRX(),cereal.GetRenderPos(1)->getRY(),cereal.GetRenderPos(1)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(1)->getScaleX(),cereal.GetRenderPos(1)->getScaleY(),cereal.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_1], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(2)->getTranslationX()+(j*1.4),cereal.GetRenderPos(2)->getTranslationY(),cereal.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(2)->getRotation(),cereal.GetRenderPos(2)->getRX(),cereal.GetRenderPos(2)->getRY(),cereal.GetRenderPos(0)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(2)->getScaleX(),cereal.GetRenderPos(2)->getScaleY(),cereal.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_1], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(3)->getTranslationX()+(j*1.4),cereal.GetRenderPos(3)->getTranslationY(),cereal.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(3)->getRotation(),cereal.GetRenderPos(3)->getRX(),cereal.GetRenderPos(3)->getRY(),cereal.GetRenderPos(3)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(3)->getScaleX(),cereal.GetRenderPos(3)->getScaleY(),cereal.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_1], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(4)->getTranslationX()+(j*1.4),cereal.GetRenderPos(4)->getTranslationY(),cereal.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(4)->getRotation(),cereal.GetRenderPos(4)->getRX(),cereal.GetRenderPos(4)->getRY(),cereal.GetRenderPos(4)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(4)->getScaleX(),cereal.GetRenderPos(4)->getScaleY(),cereal.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_1], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(5)->getTranslationX()+(j*1.4),cereal.GetRenderPos(5)->getTranslationY(),cereal.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(5)->getRotation(),cereal.GetRenderPos(5)->getRX(),cereal.GetRenderPos(5)->getRY(),cereal.GetRenderPos(5)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(5)->getScaleX(),cereal.GetRenderPos(5)->getScaleY(),cereal.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_1], true);
			modelStack.PopMatrix();
		}
	}

	//shelf1-2
	
	modelStack.Translate(5.9,0,0);
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(0)->getTranslationX()+(j*1.4),cereal.GetRenderPos(0)->getTranslationY(),cereal.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(0)->getRotation()-90,cereal.GetRenderPos(0)->getRX(),cereal.GetRenderPos(0)->getRY(),cereal.GetRenderPos(0)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(0)->getScaleX(),cereal.GetRenderPos(0)->getScaleY(),cereal.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_3], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(1)->getTranslationX()+(j*1.4),cereal.GetRenderPos(1)->getTranslationY(),cereal.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(1)->getRotation()-90,cereal.GetRenderPos(1)->getRX(),cereal.GetRenderPos(1)->getRY(),cereal.GetRenderPos(1)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(1)->getScaleX(),cereal.GetRenderPos(1)->getScaleY(),cereal.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_3], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(2)->getTranslationX()+(j*1.4),cereal.GetRenderPos(2)->getTranslationY(),cereal.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(2)->getRotation()-90,cereal.GetRenderPos(2)->getRX(),cereal.GetRenderPos(2)->getRY(),cereal.GetRenderPos(0)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(2)->getScaleX(),cereal.GetRenderPos(2)->getScaleY(),cereal.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_3], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(3)->getTranslationX()+(j*1.4),cereal.GetRenderPos(3)->getTranslationY(),cereal.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(3)->getRotation()-90,cereal.GetRenderPos(3)->getRX(),cereal.GetRenderPos(3)->getRY(),cereal.GetRenderPos(3)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(3)->getScaleX(),cereal.GetRenderPos(3)->getScaleY(),cereal.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_3], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(4)->getTranslationX()+(j*1.4),cereal.GetRenderPos(4)->getTranslationY(),cereal.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(4)->getRotation()-90,cereal.GetRenderPos(4)->getRX(),cereal.GetRenderPos(4)->getRY(),cereal.GetRenderPos(4)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(4)->getScaleX(),cereal.GetRenderPos(4)->getScaleY(),cereal.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_3], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(5)->getTranslationX()+(j*1.4),cereal.GetRenderPos(5)->getTranslationY(),cereal.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(5)->getRotation()-90,cereal.GetRenderPos(5)->getRX(),cereal.GetRenderPos(5)->getRY(),cereal.GetRenderPos(5)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(5)->getScaleX(),cereal.GetRenderPos(5)->getScaleY(),cereal.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_3], true);
			modelStack.PopMatrix();
		}
	}

	//shelf1-3
	modelStack.Translate(6,0,0);
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(0)->getTranslationX()+(j*1.4),cereal.GetRenderPos(0)->getTranslationY(),cereal.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(0)->getRotation()-90,cereal.GetRenderPos(0)->getRX(),cereal.GetRenderPos(0)->getRY(),cereal.GetRenderPos(0)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(0)->getScaleX(),cereal.GetRenderPos(0)->getScaleY(),cereal.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_3], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(1)->getTranslationX()+(j*1.4),cereal.GetRenderPos(1)->getTranslationY(),cereal.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(1)->getRotation()-90,cereal.GetRenderPos(1)->getRX(),cereal.GetRenderPos(1)->getRY(),cereal.GetRenderPos(1)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(1)->getScaleX(),cereal.GetRenderPos(1)->getScaleY(),cereal.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_3], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(2)->getTranslationX()+(j*1.4),cereal.GetRenderPos(2)->getTranslationY(),cereal.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(2)->getRotation()-90,cereal.GetRenderPos(2)->getRX(),cereal.GetRenderPos(2)->getRY(),cereal.GetRenderPos(0)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(2)->getScaleX(),cereal.GetRenderPos(2)->getScaleY(),cereal.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_3], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(3)->getTranslationX()+(j*1.4),cereal.GetRenderPos(3)->getTranslationY(),cereal.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(3)->getRotation()-90,cereal.GetRenderPos(3)->getRX(),cereal.GetRenderPos(3)->getRY(),cereal.GetRenderPos(3)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(3)->getScaleX(),cereal.GetRenderPos(3)->getScaleY(),cereal.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_3], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(4)->getTranslationX()+(j*1.4),cereal.GetRenderPos(4)->getTranslationY(),cereal.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(4)->getRotation()-90,cereal.GetRenderPos(4)->getRX(),cereal.GetRenderPos(4)->getRY(),cereal.GetRenderPos(4)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(4)->getScaleX(),cereal.GetRenderPos(4)->getScaleY(),cereal.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_3], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(5)->getTranslationX()+(j*1.4),cereal.GetRenderPos(5)->getTranslationY(),cereal.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(5)->getRotation()-90,cereal.GetRenderPos(5)->getRX(),cereal.GetRenderPos(5)->getRY(),cereal.GetRenderPos(5)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(5)->getScaleX(),cereal.GetRenderPos(5)->getScaleY(),cereal.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_3], true);
			modelStack.PopMatrix();
		}
	}

	//Shelf1-4
	//Left Side
	modelStack.Translate(6,0,0);
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(0)->getTranslationX()+(j*1.4),cereal.GetRenderPos(0)->getTranslationY(),cereal.GetRenderPos(0)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(0)->getRotation(),cereal.GetRenderPos(0)->getRX(),cereal.GetRenderPos(0)->getRY(),cereal.GetRenderPos(0)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(0)->getScaleX(),cereal.GetRenderPos(0)->getScaleY(),cereal.GetRenderPos(0)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_2], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(1)->getTranslationX()+(j*1.4),cereal.GetRenderPos(1)->getTranslationY(),cereal.GetRenderPos(1)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(1)->getRotation(),cereal.GetRenderPos(1)->getRX(),cereal.GetRenderPos(1)->getRY(),cereal.GetRenderPos(1)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(1)->getScaleX(),cereal.GetRenderPos(1)->getScaleY(),cereal.GetRenderPos(1)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_2], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(2)->getTranslationX()+(j*1.4),cereal.GetRenderPos(2)->getTranslationY(),cereal.GetRenderPos(2)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(2)->getRotation(),cereal.GetRenderPos(2)->getRX(),cereal.GetRenderPos(2)->getRY(),cereal.GetRenderPos(0)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(2)->getScaleX(),cereal.GetRenderPos(2)->getScaleY(),cereal.GetRenderPos(2)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_2], true);
			modelStack.PopMatrix();
		}
	}

	//RIGHT Side
	for(int i = 1; i <= 2; ++ i )
	{
		//BTM
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(3)->getTranslationX()+(j*1.4),cereal.GetRenderPos(3)->getTranslationY(),cereal.GetRenderPos(3)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(3)->getRotation(),cereal.GetRenderPos(3)->getRX(),cereal.GetRenderPos(3)->getRY(),cereal.GetRenderPos(3)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(3)->getScaleX(),cereal.GetRenderPos(3)->getScaleY(),cereal.GetRenderPos(3)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_2], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//MIDDLE
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(4)->getTranslationX()+(j*1.4),cereal.GetRenderPos(4)->getTranslationY(),cereal.GetRenderPos(4)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(4)->getRotation(),cereal.GetRenderPos(4)->getRX(),cereal.GetRenderPos(4)->getRY(),cereal.GetRenderPos(4)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(4)->getScaleX(),cereal.GetRenderPos(4)->getScaleY(),cereal.GetRenderPos(4)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_2], true);
			modelStack.PopMatrix();
		}
	}

	for(int i = 1; i <= 2; ++ i )
	{
		//TOP
		for(int j = 0; j < 4; ++ j )
		{
			modelStack.PushMatrix();
			modelStack.Translate(cereal.GetRenderPos(5)->getTranslationX()+(j*1.4),cereal.GetRenderPos(5)->getTranslationY(),cereal.GetRenderPos(5)->getTranslationZ()+(i*0.6));
			modelStack.Rotate(cereal.GetRenderPos(5)->getRotation(),cereal.GetRenderPos(5)->getRX(),cereal.GetRenderPos(5)->getRY(),cereal.GetRenderPos(5)->getRZ());
			modelStack.Scale(cereal.GetRenderPos(5)->getScaleX(),cereal.GetRenderPos(5)->getScaleY(),cereal.GetRenderPos(5)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CEREAL_2], true);
			modelStack.PopMatrix();
		}
	}

	modelStack.PopMatrix();
}

void SP2::RenderPizza()
{
	for(int j = 0; j < 7; ++j)
	{
		for(int i = 0; i < 4; ++i)
		{
			modelStack.PushMatrix();
			modelStack.Translate(pizza.GetRenderPos(j)->getTranslationX()+(i*3),pizza.GetRenderPos(j)->getTranslationY(),pizza.GetRenderPos(j)->getTranslationZ());
			modelStack.Rotate(pizza.GetRenderPos(j)->getRotation(),pizza.GetRenderPos(j)->getRX(),pizza.GetRenderPos(j)->getRY(),pizza.GetRenderPos(j)->getRZ());
			modelStack.Scale(pizza.GetRenderPos(j)->getScaleX(),pizza.GetRenderPos(j)->getScaleY(),pizza.GetRenderPos(j)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_PIZZA], true);
			modelStack.PopMatrix();
		}
	}

}

void SP2::RenderIceCream()
{
	for(int j = 0; j < 2; ++j)
	{
		for(int i = 0; i < 6; ++i)
		{
			modelStack.PushMatrix();
			modelStack.Translate(icecream.GetRenderPos(j)->getTranslationX()-(i*2),icecream.GetRenderPos(j)->getTranslationY(),icecream.GetRenderPos(j)->getTranslationZ());
			modelStack.Rotate(icecream.GetRenderPos(j)->getRotation(),icecream.GetRenderPos(j)->getRX(),icecream.GetRenderPos(j)->getRY(),icecream.GetRenderPos(j)->getRZ());
			modelStack.Scale(icecream.GetRenderPos(j)->getScaleX(),icecream.GetRenderPos(j)->getScaleY(),icecream.GetRenderPos(j)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_ICECREAM], true);
			modelStack.PopMatrix();
		}
	}
}

void SP2::RenderMcNCheese()
{
	for(int j = 0; j < 2; ++j)
	{
		for(int i = 0; i < 8; ++i)
		{
			modelStack.PushMatrix();
			modelStack.Translate(mcncheese.GetRenderPos(j)->getTranslationX()-(i*1.5),mcncheese.GetRenderPos(j)->getTranslationY(),mcncheese.GetRenderPos(j)->getTranslationZ());
			modelStack.Rotate(mcncheese.GetRenderPos(j)->getRotation(),mcncheese.GetRenderPos(j)->getRX(),mcncheese.GetRenderPos(j)->getRY(),mcncheese.GetRenderPos(j)->getRZ());
			modelStack.Scale(mcncheese.GetRenderPos(j)->getScaleX(),mcncheese.GetRenderPos(j)->getScaleY(),mcncheese.GetRenderPos(j)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_MCNCHEESE], true);
			modelStack.PopMatrix();
		}
	}
}

void SP2::RenderSardines()
{
	for(int j = 0; j < 2; ++j)
	{
		for(int i = 0; i < 9; ++i)
		{
			modelStack.PushMatrix();
			modelStack.Translate(sardines.GetRenderPos(j)->getTranslationX()-(i*1.5),sardines.GetRenderPos(j)->getTranslationY(),sardines.GetRenderPos(j)->getTranslationZ());
			modelStack.Rotate(sardines.GetRenderPos(j)->getRotation(),sardines.GetRenderPos(j)->getRX(),sardines.GetRenderPos(j)->getRY(),sardines.GetRenderPos(j)->getRZ());
			modelStack.Scale(sardines.GetRenderPos(j)->getScaleX(),sardines.GetRenderPos(j)->getScaleY(),sardines.GetRenderPos(j)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_SARDINES], true);
			modelStack.PopMatrix();
		}
	}
}

void SP2::RenderAyamCan()
{
	for (int j = 0; j < 2; ++j)
	{
		for(int i = 0; i < 13; ++i)
		{
			modelStack.PushMatrix();
			modelStack.Translate(ayamcan.GetRenderPos(j)->getTranslationX()-(i*1),ayamcan.GetRenderPos(j)->getTranslationY(),ayamcan.GetRenderPos(j)->getTranslationZ());
			modelStack.Rotate(ayamcan.GetRenderPos(j)->getRotation(),ayamcan.GetRenderPos(j)->getRX(),ayamcan.GetRenderPos(j)->getRY(),ayamcan.GetRenderPos(j)->getRZ());
			modelStack.Scale(ayamcan.GetRenderPos(j)->getScaleX(),ayamcan.GetRenderPos(j)->getScaleY(),ayamcan.GetRenderPos(j)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_AYAMCAN], true);
			modelStack.PopMatrix();
		}
	}
}

void SP2::RenderChickenStock()
{
	for(int j = 0; j < 2; ++j)
	{
		for(int i = 0; i < 7; ++i)
		{
			modelStack.PushMatrix();
			modelStack.Translate(chickenstock.GetRenderPos(j)->getTranslationX()-(i*1.8),chickenstock.GetRenderPos(j)->getTranslationY(),chickenstock.GetRenderPos(j)->getTranslationZ());
			modelStack.Rotate(chickenstock.GetRenderPos(j)->getRotation(),chickenstock.GetRenderPos(j)->getRX(),chickenstock.GetRenderPos(j)->getRY(),chickenstock.GetRenderPos(j)->getRZ());
			modelStack.Scale(chickenstock.GetRenderPos(j)->getScaleX(),chickenstock.GetRenderPos(j)->getScaleY(),chickenstock.GetRenderPos(j)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_CHICKENSTOCK], true);
			modelStack.PopMatrix();
		}
	}
}

void SP2::RenderToblerone()
{
	for(int j = 0; j < 4; ++j)
	{
		for(int i = 0; i < 4; ++i)
		{
			modelStack.PushMatrix();
			modelStack.Translate(toblerone.GetRenderPos(j)->getTranslationX()-(i*3),toblerone.GetRenderPos(j)->getTranslationY(),toblerone.GetRenderPos(j)->getTranslationZ());
			modelStack.Rotate(toblerone.GetRenderPos(j)->getRotation(),toblerone.GetRenderPos(j)->getRX(),toblerone.GetRenderPos(j)->getRY(),toblerone.GetRenderPos(j)->getRZ());
			modelStack.Scale(toblerone.GetRenderPos(j)->getScaleX(),toblerone.GetRenderPos(j)->getScaleY(),toblerone.GetRenderPos(j)->getScaleZ());
			RenderMesh(meshList[GEO_MODEL_TOBLERONE], true);
			modelStack.PopMatrix();
		}
	}
}

void SP2::RenderWorld()
{
	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(0)->getTranslationX(),data.GetRenderPos(0)->getTranslationY(),data.GetRenderPos(0)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(0)->getRotation(),data.GetRenderPos(0)->getRX(),data.GetRenderPos(0)->getRY(),data.GetRenderPos(0)->getRZ());
	modelStack.Scale(data.GetRenderPos(0)->getScaleX(),data.GetRenderPos(0)->getScaleY(),data.GetRenderPos(0)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_MART], true);

	modelStack.PushMatrix();
	modelStack.Translate(EntranceDoorSlide,data.GetRenderPos(1)->getTranslationY(),data.GetRenderPos(1)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(1)->getRotation(),data.GetRenderPos(1)->getRX(),data.GetRenderPos(1)->getRY(),data.GetRenderPos(1)->getRZ());
	modelStack.Scale(data.GetRenderPos(1)->getScaleX(),data.GetRenderPos(1)->getScaleY(),data.GetRenderPos(1)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_DOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(ExitDoorSlide,data.GetRenderPos(2)->getTranslationY(),data.GetRenderPos(2)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(2)->getRotation(),data.GetRenderPos(2)->getRX(),data.GetRenderPos(2)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(2)->getScaleX(),data.GetRenderPos(2)->getScaleY(),data.GetRenderPos(2)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_DOOR], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(data.GetRenderPos(3)->getTranslationX(),data.GetRenderPos(3)->getTranslationY(),data.GetRenderPos(3)->getTranslationZ());
	modelStack.Rotate(data.GetRenderPos(3)->getRotation(),data.GetRenderPos(3)->getRX(),data.GetRenderPos(3)->getRY(),data.GetRenderPos(2)->getRZ());
	modelStack.Scale(data.GetRenderPos(3)->getScaleX(),data.GetRenderPos(3)->getScaleY(),data.GetRenderPos(3)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_FLOOR], true);
	modelStack.PopMatrix();
	modelStack.PopMatrix();
}

void SP2::RenderCharacter()
{
	modelStack.PushMatrix();
	modelStack.Translate(npc.getPosX(),character.GetRenderPos(0)->getTranslationY(),npc.getPosZ());
	modelStack.Rotate(npc.getRot(),character.GetRenderPos(0)->getRX(),character.GetRenderPos(0)->getRY(),character.GetRenderPos(0)->getRZ());
	modelStack.Scale(character.GetRenderPos(0)->getScaleX(),character.GetRenderPos(0)->getScaleY(),character.GetRenderPos(0)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_DOORMAN], true);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(shopper.getPosX(),character.GetRenderPos(0)->getTranslationY(),shopper.getPosZ());
	modelStack.Rotate(shopper.getRot(),character.GetRenderPos(0)->getRX(),character.GetRenderPos(0)->getRY(),character.GetRenderPos(0)->getRZ());
	modelStack.Scale(character.GetRenderPos(0)->getScaleX(),character.GetRenderPos(0)->getScaleY(),character.GetRenderPos(0)->getScaleZ());
	RenderMesh(meshList[GEO_MODEL_DOORMAN], true);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	//modelStack.Translate(character.GetRenderPos(0)->getTranslationX(),character.GetRenderPos(0)->getTranslationY(),character.GetRenderPos(0)->getTranslationZ());
	//modelStack.Rotate(character.GetRenderPos(0)->getRotation(),character.GetRenderPos(0)->getRX(),character.GetRenderPos(0)->getRY(),character.GetRenderPos(0)->getRZ());
	//modelStack.Scale(character.GetRenderPos(0)->getScaleX(),character.GetRenderPos(0)->getScaleY(),character.GetRenderPos(0)->getScaleZ());
	//RenderMesh(meshList[GEO_MODEL_CHAR1], true);
	//modelStack.PopMatrix();
}

void SP2::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if(enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	//mesh->Render();

	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render(); //this line should only be called once 
	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SP2::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SP2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;
	
	
	glDisable(GL_DEPTH_TEST);

	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 1.0f+1.5f, 1.0f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}

void SP2::Exit()
{
	// Cleanup here
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i] != NULL)
			delete meshList[i];
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}

