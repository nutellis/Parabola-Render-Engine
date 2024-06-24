#include <GameWorld/Components/Components.h>

//-------------------------------------------------
/*ta components na uio8etoun to RelativeLocation tou root*/
//-------------------------------------------------


PComponent::PComponent()
{
//	std::cout << "This is a Component\n";
	ComponentArchive = new UArchive();
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------

PSceneComponent::PSceneComponent()
{
	RelativeLocation = glm::vec3(0.0f);
	RelativeRotation = glm::vec3(0.0f,1.0f,0.0f);
	RelativeScale = glm::vec3(1.0f);

	SetAbsolute(true, true, true);

//	std::cout << "This is a SceneComponent with RelativeLocation ("<<RelativeLocation.x<<","<<RelativeLocation.y <<","<<RelativeLocation.z<<")\n";
}

void PSceneComponent::SetAbsolute(bool bNewAbsoluteLocation, bool bNewAbsoluteRotation, bool bNewAbsoluteScale)
{
	bAbsoluteLocation = bNewAbsoluteLocation;
	bAbsoluteRotation = bNewAbsoluteRotation;
	bAbsoluteScale = bNewAbsoluteScale;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//								STATIC MESH COMPONENT
//----------------------------------------------------------------------------------------
PStaticMeshComponent::PStaticMeshComponent()//const char* path,PSceneComponent *Default,bool isAbsolute)
{
	std::cout << "This is a StaticMeshComponent\n";
//	ComponentArchive->Load(path);
//	Deserialize(*ComponentArchive);

	RelativeLocation = glm::vec3(0.0);//Default->RelativeLocation;
		RelativeRotation = glm::vec3(0.0); //Default->RelativeRotation;
		RelativeScale = glm::vec3(1.0); //Default->RelativeScale;

		SetAbsolute(false, false, false);//isAbsolute, isAbsolute, isAbsolute);

	//std::cout <<"X "<< cMaterial->mDiffuse->ch_Colour.x << std::endl;
	//std::cout <<"Y "<< cMaterial->mDiffuse->ch_Colour.y << std::endl;
	//std::cout <<"Z "<< cMaterial->mDiffuse->ch_Colour.z << std::endl;
	//angle = 45.0f;
}
void PStaticMeshComponent::SetShader(Shader shader)
{
	int	TextureSlot = 0;
	/* set up everything for this mesh*/

	/*Set Emmisive*/
	/*if (cMaterial->mEmissive->HasTexture) {
		glActiveTexture(GL_TEXTURE0 + TextureSlot);
		glBindTexture(GL_TEXTURE_2D, cMaterial->mEmissive->ch_Texture->textureID);
		TextureSlot++;
	}
	shader.setVec4(cMaterial->mEmissive->ChName, cMaterial->mEmissive->ch_Colour);

	/* model view*/
//	shader.setVec3("objectColor",glm::vec3(cMaterial->mDiffuse->ch_Colour));
	
	glm::mat4 model;
	if (this->RelativeRotation.y != 0.0) {
		//update root rotation and location.


		//model = glm::rotate(model, angle, this->RelativeRotation);
	}

	model = glm::translate(model,this->RelativeLocation);
	model = glm::scale(model, this->RelativeScale);
	shader.setMat4("model", model);
	angle += 0.009f;
}
void PStaticMeshComponent::Deserialize(UArchive & Ar)
{
	std::cout << "LOL" << std::endl;
	Mesh = new StaticMesh(Ar);
	cMaterial = new Material(Ar);
	cMaterial->SetupMaterial();

}
void PStaticMeshComponent::BindTextures(Shader shader)
{
	cMaterial;
	glActiveTexture(GL_TEXTURE0);
	//glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
	// and finally bind the texture
	//glBindTexture(GL_TEXTURE_2D, textures[i].id);
}
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//									POINT LIGHT COMPONENT
//----------------------------------------------------------------------------------------

PPointLightComponent::PPointLightComponent()
{
	//glm::vec3(1.2f, 1.0f, 2.0f);
	std::cout << "This is a PointLightComponent\n";
	Attributes = new PointLightAttributes();
}

PPointLightComponent::PPointLightComponent(PSceneComponent * Default)
{

	RelativeLocation = Default->RelativeLocation;
	RelativeRotation = Default->RelativeRotation;
	RelativeScale = Default->RelativeScale;

	std::cout << "This is a PointLightComponent\n";
	Attributes = new PointLightAttributes();

}

PPointLightComponent::~PPointLightComponent()
{
	delete[] Attributes;
}

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//									CAMERA COMPONENT
//----------------------------------------------------------------------------------------
PCameraComponent::PCameraComponent(glm::vec3 position, glm::vec3 up, float yaw, float pitch) 
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	RelativeLocation = position;
	WorldUp = up;
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

PCameraComponent::PCameraComponent(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
	: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	RelativeLocation = glm::vec3(posX, posY, posZ);
	WorldUp = glm::vec3(upX, upY, upZ);
	Yaw = yaw;
	Pitch = pitch;
	updateCameraVectors();
}

void PCameraComponent::SetShader(Shader shader)
{

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
	glm::mat4 view = GetViewMatrix();
	shader.setMat4("projection", projection);
	shader.setMat4("view", view);
	
	//view position for lighting;
	//shader.setVec3("viewPos", RelativeLocation);
}

glm::mat4 PCameraComponent::GetViewMatrix()
{
	return glm::lookAt(RelativeLocation, RelativeLocation + Front, Up);
}

void PCameraComponent::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		RelativeLocation += Front * velocity;
	if (direction == BACKWARD)
		RelativeLocation -= Front * velocity;
	if (direction == LEFT)
		RelativeLocation -= Right * velocity;
	if (direction == RIGHT)
		RelativeLocation += Right * velocity;
}

void PCameraComponent::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	updateCameraVectors();
}

void PCameraComponent::ProcessMouseScroll(float yoffset)
{
	if (Zoom >= 1.0f && Zoom <= 45.0f)
		Zoom -= yoffset;
	if (Zoom <= 1.0f)
		Zoom = 1.0f;
	if (Zoom >= 45.0f)
		Zoom = 45.0f;
}

void PCameraComponent::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = glm::normalize(glm::cross(Right, Front));
}



//class InputComponent
//{
//public:
//	InputComponent();
//	~InputComponent();
//
//
//	bool isKeyPressed(uint32 keycode) const;
//	bool isMouseButtonPressed(uint32 button) const;
//	void getMousePosition(double &x, double &y) const;
//
//	void processInput(GLFWwindow *window);
//
//
//private:
//
//	Window *activeWin;
//
//	bool wKeys[MAX_KEYS];
//	bool wMouseButtons[MAX_BUTTONS];
//	double mPosX, mPosY;
//
//
//private:
//
//	friend static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
//	friend static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
//	friend static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos);
//
//
//};

//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------