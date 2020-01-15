
#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2); // VTK was built with vtkRenderingOpenGL2
VTK_MODULE_INIT(vtkInteractionStyle);
#define vtkRenderingCore_AUTOINIT2(vtkRenderingOpenGL2, vtkInteractionStyle)

#include <math.h>       /* pow */
#include<iostream>
#include <vtkSmartPointer.h>
#include <vtkCubeSource.h>
#include <vtkLineSource.h>
#include <vtkCamera.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkActor2D.h>
#include <vtkCommand.h>
#include <vtkRenderer.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include<vtkJPEGReader.h>
#include<vtkImageActor.h>
#include <vtktransform.h>
#include <vtkSphereSource.h>
#include <vtkRegularPolygonSource.h>
#include <vtkTransformTextureCoords.h>
#include <vtkTexturedSphereSource.h>
#include <vtkImageReader2Factory.h>
#include <vtkImageReader2.h>
#include <vtkSuperquadricSource.h>
#include<vtkImageData.h>
#include<vtkTensorProbeRepresentation.h>
#include<Windows.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include<vtkDelaunay3D.h>
#include<vtkUnstructuredGrid.h>
#pragma comment(lib,"winmm.lib")



void play_music_bg() {
	PlaySound(TEXT("Assets/Musics/BeethovenSilence.wav"), NULL, SND_LOOP | SND_ASYNC);
}

void play_music_exp() {
	PlaySound(TEXT("Assets/Musics/sound.wav"), NULL, SND_LOOP | SND_ASYNC);
}

void Cam(vtkActor* actor, vtkCamera* camera, bool Soleil);
const float RadiusSoleil = 3.5;
const int Const = 50000;
int boolPlanete = 0;
float p = 0;
double LePas;
//Camera
/**/	vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
std::vector<vtkSmartPointer<vtkTexturedSphereSource>> myPlanets;
void play_music_bg();
class vtkTimerCallback2 : public vtkCommand
{
public:
	float getAngleSoitMeme() {
		return AngleSoitMeme;
	}
	void setAngleSoitMeme(float val) {
		AngleSoitMeme = val;
	}
	int getType() {
		return Type;
	}
	void setType(int val) {
		Type = val;
	}

	void setBoolean(bool val) {
		boolean = val;
	}
	static vtkTimerCallback2* New() {
		vtkTimerCallback2* cb = new vtkTimerCallback2;
		cb->AngleSoitMeme = 0;
		cb->AngleAutourSoleil = 0;
		return cb;
	}

	virtual void Execute(vtkObject* caller, unsigned long eventId, void* vtkNotUsed(callData)) {
		vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
		vtkSmartPointer<vtkPoints> pointsInside = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkPolyData> polyDataInside = vtkSmartPointer<vtkPolyData>::New();
		double point[3];
		vtkSmartPointer<vtkDelaunay3D> delaunay3D = vtkSmartPointer<vtkDelaunay3D>::New();
		if (vtkCommand::TimerEvent == eventId) {
			switch (this->Type) {
			case 0: //variables x et y du commet
				double Tab[3];
				actor->GetPosition(Tab);
				this->Trajectoirex -= Tab[0] * LePas;
				this->Trajectoirey -= Tab[1] * LePas;
				this->Trajectoirez -= Tab[2] * LePas;
				//transform->PostMultiply();		 //this is the key line

				transform->Translate(this->Trajectoirex, this->Trajectoirey, this->Trajectoirez);
				actor->SetUserTransform(transform);




				vtkMatrix4x4* m;			  // Représente la matrice de Trasnformation
				double Result[3];			  // Représente le Point d'arrivée ( aprés transformation)
				m = actor->GetUserMatrix();  // Représente la matrice de Trasnformation

				delaunay3D->SetInputConnection(myPlanets.at(9)->GetOutputPort());
				delaunay3D->Update();
				cout << "  hello" << endl;
				for (vtkIdType i = 0; i < myPlanets.at(0)->GetOutput()->GetNumberOfPoints(); i++)
				{
					myPlanets.at(0)->GetOutput()->GetPoint(i, point);

					Result[0] = m->GetElement(0, 0) * (point[0] + Tab[0]) + m->GetElement(0, 1) * (point[1] + Tab[1]) + m->GetElement(0, 2) * (point[2] + Tab[2]) + m->GetElement(0, 3);
					Result[1] = m->GetElement(1, 0) * (point[0] + Tab[0]) + m->GetElement(1, 1) * (point[1] + Tab[1]) + m->GetElement(1, 2) * (point[2] + Tab[2]) + m->GetElement(1, 3);
					Result[2] = m->GetElement(2, 0) * (point[0] + Tab[0]) + m->GetElement(2, 1) * (point[1] + Tab[1]) + m->GetElement(2, 2) * (point[2] + Tab[2]) + m->GetElement(2, 3);

					double pcoords[3];
					// The number of weights >= max number of nodes in each cell
					double weights[4];

					int subId;
					// verifier si delaunay == bigSpher contient un point de la petite sphere
					vtkIdType cellId = delaunay3D->GetOutput()->FindCell(Result, NULL, 0, .1, subId, pcoords, weights);
					//if (i == 1)
					if (cellId >= 0) {
						play_music_exp();
						std::cout << "collision ................................" << cellId << std::endl;
						actor->VisibilityOff();
					}
					else
					{
						//std::cout << "pas de collision" << std::endl;
					}
				}

				break;
			case 1:  //Soleil  Sur lui meme 27jours
				this->AngleSoitMeme = -(float)360 / (float)2332800 * Const;
				cpt += this->AngleSoitMeme;
				JourTerreNew = -cpt / 360;
				if (JourTerreOld != JourTerreNew) {
					//					std::cout << "Tour Soleil (27 jours) -> " << JourTerreNew << std::endl;
					JourTerreOld = JourTerreNew;
				}
				actor->RotateZ(this->AngleSoitMeme);
				if (boolPlanete == 1) {
					Cam(actor, camera, true);
				}
				break;


			case 2://Mercure  Sur elle meme 59j   sur le soleil 88 jours 
				//Sur elle meme 
				this->AngleSoitMeme = (float)360 / (float)5097600 * Const;
				cpt += this->AngleSoitMeme;
				JourTerreNew = cpt / 360;
				if (JourTerreOld != JourTerreNew) {
					//					std::cout << "Tour Mercure (59 jours sur soit meme)-> " << JourTerreNew << std::endl;
					JourTerreOld = JourTerreNew;
				}
				//Autour du soleil 
				transform->PostMultiply();		 //this is the key line
				this->AngleAutourSoleil += (float)360 / (float)7603200 * Const;
				transform->RotateZ(-this->AngleAutourSoleil);
				actor->SetUserTransform(transform);
				actor->RotateZ(-this->AngleSoitMeme);
				actor->RotateY(-this->AngleSoitMeme);
				if (boolPlanete == 2) {
					Cam(actor, camera, false);
				}
				break;

			case 3://Venus  Sur elle meme 243j   sur le soleil 224 jours 
				//Sur elle meme 
				this->AngleSoitMeme = (float)360 / (float)20995200 * Const;
				cpt += this->AngleSoitMeme;
				JourTerreNew = cpt / 360;
				if (JourTerreOld != JourTerreNew) {
					//					std::cout << "Tour Venus (243 jours sur soit meme)-> " << JourTerreNew << std::endl;
					JourTerreOld = JourTerreNew;
				}
				//Autour du soleil 
				transform->PostMultiply();		 //this is the key line
				this->AngleAutourSoleil += (float)360 / (float)19353600 * Const;
				transform->RotateZ(-this->AngleAutourSoleil);
				actor->RotateZ(-this->AngleSoitMeme);
				actor->SetUserTransform(transform);
				if (boolPlanete == 3) {
					Cam(actor, camera, false);
				}
				break;

			case 4:	//Terre  Sur elle meme 24h   sur le soleil 365 jours 
				transform->PostMultiply();		 //this is the key line
				//Sur elle meme 
				this->AngleSoitMeme = (float)360 / (float)86400 * Const;
				cpt += this->AngleSoitMeme;
				JourTerreNew = cpt / 360;
				if (JourTerreOld != JourTerreNew) {
					//					std::cout << "Jour Terre -> " << JourTerreNew << std::endl;
					JourTerreOld = JourTerreNew;
				}
				//Autour du soleil 

				this->AngleAutourSoleil += (float)360 / (float)31536000 * Const;
				transform->RotateZ(-this->AngleAutourSoleil);
				actor->RotateZ(-this->AngleSoitMeme);
				actor->SetUserTransform(transform);
				if (boolPlanete == 4) {
					Cam(actor, camera, false);
				}
				break;

			case 5:	//Mars Sur elle meme 24,61h   sur le soleil 687 jours 
			//Sur elle meme 
				this->AngleSoitMeme = (float)360 / (float)88596 * Const;
				cpt += this->AngleSoitMeme;
				JourTerreNew = cpt / 360;
				if (JourTerreOld != JourTerreNew) {
					//std::cout << "Tour Venus(24,61h sur soit meme)-> " << JourTerreNew << std::endl;
					JourTerreOld = JourTerreNew;
				}
				//Autour du soleil 
				transform->PostMultiply();		 //this is the key line
				this->AngleAutourSoleil += (float)360 / (float)59356800 * Const;
				transform->RotateZ(-this->AngleAutourSoleil);
				actor->RotateZ(-this->AngleSoitMeme);
				actor->SetUserTransform(transform);
				if (boolPlanete == 5) {
					Cam(actor, camera, false);
				}
				break;

			case 6:	//Mars Sur elle meme 9,83 h   sur le soleil 4 332,5 jours 
			//Sur elle meme 
				this->AngleSoitMeme = (float)360 / (float)35388 * Const;
				cpt += this->AngleSoitMeme;
				JourTerreNew = cpt / 360;
				if (JourTerreOld != JourTerreNew) {
					//std::cout << "Tour Venus(24,61h sur soit meme)-> " << JourTerreNew << std::endl;
					JourTerreOld = JourTerreNew;
				}
				//Autour du soleil 
				transform->PostMultiply();		 //this is the key line
				this->AngleAutourSoleil += (float)360 / (float)374328000 * Const;
				transform->RotateZ(-this->AngleAutourSoleil);
				actor->RotateZ(-this->AngleSoitMeme);
				actor->SetUserTransform(transform);
				if (boolPlanete == 6) {
					Cam(actor, camera, false);
				}
				break;

			case 7:	//Saturne Sur elle meme 10,23 h   sur le soleil 10 759,23 jours
				//Sur elle meme 
				this->AngleSoitMeme = (float)360 / (float)36828 * Const;
				cpt += this->AngleSoitMeme;
				JourTerreNew = cpt / 360;
				if (JourTerreOld != JourTerreNew) {
					//std::cout << "Tour Venus(24,61h sur soit meme)-> " << JourTerreNew << std::endl;
					JourTerreOld = JourTerreNew;
				}
				//Autour du soleil 
				transform->PostMultiply();		 //this is the key line
				this->AngleAutourSoleil += (float)360 / (float)929597472 * Const;
				transform->RotateZ(-this->AngleAutourSoleil);
				actor->RotateZ(-this->AngleSoitMeme);
				actor->SetUserTransform(transform);
				if (boolPlanete == 7) {
					Cam(actor, camera, false);
				}
				break;

			case 8:	//Uranus Sur elle meme -17 h 14 min   sur le soleil 30 685,4 jours
			//Sur elle meme 
				this->AngleSoitMeme = (float)360 / (float)25320 * Const;
				cpt += this->AngleSoitMeme;
				JourTerreNew = cpt / 360;
				if (JourTerreOld != JourTerreNew) {
					//std::cout << "Tour Venus(24,61h sur soit meme)-> " << JourTerreNew << std::endl;
					JourTerreOld = JourTerreNew;
				}
				//Autour du soleil 
				transform->PostMultiply();		 //this is the key line
				this->AngleAutourSoleil += (float)360 / (float)2651218560 * Const;
				transform->RotateZ(-this->AngleAutourSoleil);
				actor->RotateZ(-this->AngleSoitMeme);
				actor->SetUserTransform(transform);
				if (boolPlanete == 8) {
					Cam(actor, camera, false);
				}
				break;

			case 9:	//Neptune Sur elle meme 18,2 h   sur le soleil 60 266 jours
			//Sur elle meme 
				this->AngleSoitMeme = (float)360 / (float)65520 * Const;
				cpt += this->AngleSoitMeme;
				JourTerreNew = cpt / 360;
				if (JourTerreOld != JourTerreNew) {
					//std::cout << "Tour Venus(24,61h sur soit meme)-> " << JourTerreNew << std::endl;
					JourTerreOld = JourTerreNew;
				}
				//Autour du soleil 
				transform->PostMultiply();		 //this is the key line
				this->AngleAutourSoleil += (float)360 / (float)5206982400 * Const;
				transform->RotateZ(-this->AngleAutourSoleil);
				actor->RotateZ(-this->AngleSoitMeme);
				actor->SetUserTransform(transform);
				if (boolPlanete == 9) {
					Cam(actor, camera, false);
				}
				break;

			case 10://Lune  Sur elle meme 29,5j sur la terre 27.32j   sur le soleil 365 jours 
			//Sur elle meme 
				this->AngleSoitMeme = (float)360 / (float)2548800 * Const;
				cpt += this->AngleSoitMeme;
				JourTerreNew = cpt / 360;
				if (JourTerreOld != JourTerreNew) {
					//					std::cout << "Tour Mercure (59 jours sur soit meme)-> " << JourTerreNew << std::endl;
					JourTerreOld = JourTerreNew;
				}
				//Autour du soleil 
				transform->PostMultiply();		 //this is the key line
				this->AngleAutourTerre += (float)360 / (float)2360448 * Const;
				transform->RotateZ(-this->AngleAutourTerre);
				transform->RotateY(30);
				transform->Translate(-(5 + RadiusSoleil), 0, 0);
				this->AngleAutourSoleil += (float)360 / (float)31536000 * Const;
				transform->RotateZ(-this->AngleAutourSoleil);

				actor->SetUserTransform(transform);
				actor->RotateZ(+this->AngleSoitMeme);
				break;

			case 11://Orbite de la Lune meme chose que pour la terre
				//Autour du soleil 
				transform->PostMultiply();		 //this is the key line
				this->AngleAutourSoleil += (float)360 / (float)31536000 * Const;
				transform->RotateY(30);
				transform->Translate(-5 - RadiusSoleil, 0, 0);
				transform->RotateZ(-this->AngleAutourSoleil);
				//transform->GetPosition
				actor->SetUserTransform(transform);
				break;
			default:
				break;
			}
		}




		//	std::cout << "" <<this->AngleSoitMeme << std::endl;
		vtkRenderWindowInteractor* iren = vtkRenderWindowInteractor::SafeDownCast(caller);
		iren->GetRenderWindow()->Render();
	}

private:
	int JourTerreOld = 0;
	int JourTerreNew = 0;
	float cpt = 0;
	int Type = 0;
	float AngleSoitMeme;
	float AngleAutourSoleil;
	float AngleAutourTerre = 0;
	bool boolean = false;
	float Trajectoirex = 0.0;
	float Trajectoirey = 0.0;
	float Trajectoirez = 0.0;
public:
	vtkActor* actor;
};




// Methode
vtkSmartPointer<vtkTimerCallback2> getCbTerre();
vtkSmartPointer<vtkActor> PLanet(const char* imagepath, float size, float PositionX);
vtkSmartPointer<vtkActor> PLanet(const char* imagepath, float size, float PositionX, float PositionY, float PositionZ);
// Instanciation of Actors Of Orbits ans Planet
vtkSmartPointer<vtkActor> actorComete = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actor1 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actor2 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actorc2 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actor3 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actorc3 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actor4 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actorc4 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actor5 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actorc5 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actor6 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actorc6 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actor7 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actorc7 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actor8 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actorc8 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actor9 = vtkSmartPointer<vtkActor>::New();
/**/	vtkSmartPointer<vtkActor> actorc9 = vtkSmartPointer<vtkActor>::New();

//  renderer
/**/	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
/**/	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
/**/	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();

// Define interaction style
class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
	static KeyPressInteractorStyle* New();
	vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);

	virtual void OnKeyPress() {
		// Get the keypress
		vtkRenderWindowInteractor* rwi = this->Interactor;
		std::string key = rwi->GetKeySym();
		if (key == "0") { // Création de la comète aléatoirement
			float positionX = rand() % 55 + 35;
			float positionY = rand() % 20 + 10;
			float positionZ = rand() % 20 + 10;
			actorComete = PLanet("Assets/Textures/Comete.jpg", 1, positionX, positionY, positionZ);
			double Dist = (float)sqrt(pow(positionX - 0, 2) + pow(positionY - 0, 2) + pow(positionZ - 0, 2));
			LePas = (float)Dist / 10000.0;
			cout << "Le PAs = " << LePas << endl;
			// Sign up to receive TimerEvent
			cb = vtkSmartPointer<vtkTimerCallback2> ::New();
			cb->setType(0);
			cb->actor = actorComete;
			renderer->AddActor(actorComete);
			renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb);
			boolPlanete = 0;
		}
		// Handle a "normal" key
		if (key >= "1" && key <= "9") {
			vtkMatrix4x4* m = vtkSmartPointer<vtkMatrix4x4>::New();			  // Représente la matrice de Trasnformation
			double Tab[3];				  // Représente le Point de départ
			double Result[3];			  // Représente le Point d'arrivée ( aprés transformation)
			if (key == "1") { //Focaliser la caéra sur la Terre  ...................................................................................?????
				boolPlanete = 1; // Pour dépacer la caméra en fonction de la terre 
			}
			if (key == "2") { //Focaliser la caméra sur la Planete Mercure
				actor2->GetPosition(Tab);     // Représente le Point de départ
				m = actor2->GetUserMatrix();  // Représente la matrice de Trasnformation
				boolPlanete = 2; // Pour dépacer la caméra en fonction de la terre 
			}
			if (key == "3") { //Focaliser la caéra sur la Venus
				actor3->GetPosition(Tab);     // Représente le Point de départ
				m = actor3->GetUserMatrix();  // Représente la matrice de Trasnformation
				boolPlanete = 3; // Pour dépacer la caméra en fonction de la terre 
			}
			if (key == "4") { //Focaliser la caéra sur la Terre
				actor4->GetPosition(Tab);     // Représente le Point de départ
				m = actor4->GetUserMatrix();  // Représente la matrice de Trasnformation
				boolPlanete = 4; // Pour dépacer la caméra en fonction de la terre 
			}
			if (key == "5") { //Focaliser la caéra sur la Mars
				actor5->GetPosition(Tab);     // Représente le Point de départ
				m = actor5->GetUserMatrix();  // Représente la matrice de Trasnformation
				boolPlanete = 5; // Pour dépacer la caméra en fonction de la terre 
			}
			if (key == "6") { //Focaliser la caéra sur la Mars
				actor6->GetPosition(Tab);     // Représente le Point de départ
				m = actor6->GetUserMatrix();  // Représente la matrice de Trasnformation
				boolPlanete = 6; // Pour dépacer la caméra en fonction de la terre 
			}
			if (key == "7") { //Focaliser la caéra sur la Mars
				actor7->GetPosition(Tab);     // Représente le Point de départ
				m = actor7->GetUserMatrix();  // Représente la matrice de Trasnformation
				boolPlanete = 7; // Pour dépacer la caméra en fonction de la terre 
			}
			if (key == "8") { //Focaliser la caéra sur la Mars
				actor8->GetPosition(Tab);     // Représente le Point de départ
				m = actor8->GetUserMatrix();  // Représente la matrice de Trasnformation
				boolPlanete = 8; // Pour dépacer la caméra en fonction de la terre 
			}
			if (key == "9") { //Focaliser la caéra sur la Mars
				actor9->GetPosition(Tab);     // Représente le Point de départ
				m = actor9->GetUserMatrix();  // Représente la matrice de Trasnformation
				boolPlanete = 9; // Pour dépacer la caméra en fonction de la terre 
			}
		}
		// Forward events
		vtkInteractorStyleTrackballCamera::OnKeyPress();
	}

	virtual void OnLeftButtonDown() {
		boolPlanete = 0;
		// Forward events
		vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
	}

	virtual void OnMiddleButtonDown() {
		boolPlanete = 0;
		// Forward events
		vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
	}

	virtual void OnRightButtonDown() {
		boolPlanete = 0;
		// Forward events
		vtkInteractorStyleTrackballCamera::OnRightButtonDown();
	}

public:
	vtkActor* actor;
	vtkSmartPointer<vtkTimerCallback2> cb;
	//vtkCamera* camera;

};
vtkStandardNewMacro(KeyPressInteractorStyle);



int main(int, char* [])
{
	/***********************************************	OBJECT 1 : Soleil	*****************************************************/
	/**/	// Creation du Soleil 
	/**/	actor1 = PLanet("Assets/Textures/Sun.jpg", RadiusSoleil, 0);
	/**/	// Sign up to receive TimerEvent
	/**/	vtkSmartPointer<vtkTimerCallback2> cb1 = vtkSmartPointer<vtkTimerCallback2>::New();
	/**/	cb1->setType(1);
	/**/	cb1->actor = actor1;
	/*********************************************	Fin OBJECT 1 : Soleil	***************************************************/

	/***********************************************	OBJECT 2 : Mercure	*****************************************************/
	/**/	// Creation de Mercure 
	/**/	actor2 = PLanet("Assets/Textures/Mercure.jpg", 1, -1.93248 - RadiusSoleil);
	/**/	// Sign up to receive TimerEvent
	/**/	vtkSmartPointer<vtkTimerCallback2> cb2 = vtkSmartPointer<vtkTimerCallback2>::New();
	/**/	cb2->setType(2);
	/**/	cb2->actor = actor2;
	/**/
	/**/	/************************************	Orbite de Mercure	******************************************/
	/**/	// Create a circle
	/**/	vtkSmartPointer<vtkRegularPolygonSource> Circle2 = vtkSmartPointer<vtkRegularPolygonSource>::New();
	/**/
	/**/	//polygonSource->GeneratePolygonOff(); // Uncomment this line to generate only the outline of the circle
	/**/	Circle2->SetNumberOfSides(50);
	/**/	Circle2->SetRadius(1.93248 + RadiusSoleil);
	/**/	Circle2->SetCenter(0, 0, 0);
	/**/
	/**/	// Create a mapper and actor
	/**/	vtkSmartPointer<vtkPolyDataMapper> mapperc2 = vtkSmartPointer<vtkPolyDataMapper>::New();
	/**/	mapperc2->SetInputConnection(Circle2->GetOutputPort());
	/**/	actorc2->SetMapper(mapperc2);
	/**/	actorc2->GetProperty()->SetColor(0, 255, 250);
	/**/	actorc2->GetProperty()->SetRepresentationToWireframe();
	/**/
	/*********************************************	Fin OBJECT 2: Mercure	***************************************************/

	/***********************************************	OBJECT 3 : Venus	*****************************************************/
	/**/	// Creation de Mercure 
	/**/	actor3 = PLanet("Assets/Textures/Venus.jpg", 0.8, 3.6106715 + RadiusSoleil);
	/**/	// Sign up to receive TimerEvent
	/**/	vtkSmartPointer<vtkTimerCallback2> cb3 = vtkSmartPointer<vtkTimerCallback2>::New();
	/**/	cb3->setType(3);
	/**/	cb3->actor = actor3;
	/**/
	/**/	/************************************	Orbite de Venus	******************************************/
	/**/	// Create a circle
	/**/	vtkSmartPointer<vtkRegularPolygonSource> Circle3 = vtkSmartPointer<vtkRegularPolygonSource>::New();
	/**/
	/**/	//polygonSource->GeneratePolygonOff(); // Uncomment this line to generate only the outline of the circle
	/**/	Circle3->SetNumberOfSides(50);
	/**/	Circle3->SetRadius(3.6106715 + RadiusSoleil);
	/**/	Circle3->SetCenter(0, 0, 0);
	/**/
	/**/	// Create a mapper and actor
	/**/	vtkSmartPointer<vtkPolyDataMapper> mapperc3 = vtkSmartPointer<vtkPolyDataMapper>::New();
	/**/	mapperc3->SetInputConnection(Circle3->GetOutputPort());
	/**/	actorc3->SetMapper(mapperc3);
	/**/	actorc3->GetProperty()->SetColor(0, 255, 250);
	/**/	actorc3->GetProperty()->SetRepresentationToWireframe();
	/**/
	/*********************************************	Fin OBJECT 3: Venus	***************************************************/

	/***********************************************	OBJECT 4: Terre	*****************************************************/
	/**/	// Creation du Terre 
	/**/	actor4 = PLanet("Assets/Textures/Terre.jpg", 0.4, -5 - RadiusSoleil);
	/**/	// Sign up to receive TimerEvent
	/**/	vtkSmartPointer<vtkTimerCallback2> cb4 = vtkSmartPointer<vtkTimerCallback2>::New();
	/**/	cb4->setAngleSoitMeme(0);
	/**/	cb4->setType(4);
	/**/	cb4->actor = actor4;
	/**/
	/**/	/************************************	Orbite de la Terre	******************************************/
	/**/	// Create a circle
	/**/	vtkSmartPointer<vtkRegularPolygonSource> Circle4 = vtkSmartPointer<vtkRegularPolygonSource>::New();
	/**/
	/**/	//polygonSource->GeneratePolygonOff(); // Uncomment this line to generate only the outline of the circle
	/**/	Circle4->SetNumberOfSides(50);
	/**/	Circle4->SetRadius(5 + RadiusSoleil);
	/**/	Circle4->SetCenter(0, 0, 0);
	/**/
	/**/	// Create a mapper and actor
	/**/	vtkSmartPointer<vtkPolyDataMapper> mapperc4 = vtkSmartPointer<vtkPolyDataMapper>::New();
	/**/	mapperc4->SetInputConnection(Circle4->GetOutputPort());
	/**/	actorc4->SetMapper(mapperc4);
	/**/	actorc4->GetProperty()->SetColor(0, 255, 250);
	/**/	actorc4->GetProperty()->SetRepresentationToWireframe();
	/**/
	/*********************************************	Fin OBJECT 4: Terre	***************************************************/

				/***********************************************	OBJECT 10: Lune	*****************************************************/
				/**/	// Creation de la lune 
	/**/	vtkSmartPointer<vtkSphereSource> Lune = vtkSmartPointer<vtkSphereSource>::New();
	/**/	Lune->SetCenter(0.0, 0.0, 0.0);
	/**/	//CubeSource->SetRadius(2.0);
	/**/	Lune->SetRadius(0.15);
	/**/	Lune->Update();
	/**/	Lune->SetProgressText("5");
	/**/
	/**/	// Create a mapper and actor
	/**/	vtkSmartPointer<vtkPolyDataMapper> mapperLune = vtkSmartPointer<vtkPolyDataMapper>::New();
	/**/	mapperLune->SetInputConnection(Lune->GetOutputPort());
	/**/	vtkSmartPointer<vtkActor> actorLune = vtkSmartPointer<vtkActor>::New();
	/**/	actorLune->SetMapper(mapperLune);
	/**/	//actor->SetScale(2, 2, 2);
	/**/	actorLune->SetPosition(-0.61282755, 0, 0);
	/**/	actorLune->GetProperty()->SetColor(0.0, 1.0, 1.0);
	/**/
	/**/	// Sign up to receive TimerEvent
	/**/	vtkSmartPointer<vtkTimerCallback2> cbLune = vtkSmartPointer<vtkTimerCallback2>::New();
	/**/	cbLune->setAngleSoitMeme(0);
	/**/	cbLune->setType(10);
	/**/	cbLune->actor = actorLune;
	/**/
	/**/	/************************************	Orbite de la Lune: Objet 11	******************************************/
	/**/
	/**/	// Create a circle
	/**/	vtkSmartPointer<vtkRegularPolygonSource> CircleLune = vtkSmartPointer<vtkRegularPolygonSource>::New();
	/**/
	/**/	//polygonSource->GeneratePolygonOff(); // Uncomment this line to generate only the outline of the circle
	/**/	CircleLune->SetNumberOfSides(50);
	/**/	CircleLune->SetRadius(0.6);
	/**/	CircleLune->SetCenter(0, 0, 0);
	/**/
	/**/	// Create a mapper and actor
	/**/	vtkSmartPointer<vtkPolyDataMapper> mappercLune = vtkSmartPointer<vtkPolyDataMapper>::New();
	/**/	mappercLune->SetInputConnection(CircleLune->GetOutputPort());
	/**/	vtkSmartPointer<vtkActor> actorcLune = vtkSmartPointer<vtkActor>::New();
	/**/	actorcLune->SetMapper(mappercLune);
	/**/	actorcLune->GetProperty()->SetColor(0, 255, 250);
	/**/	actorcLune->GetProperty()->SetRepresentationToWireframe();
	/**///	actorcLune->SetUserTransform(transform);
	/**/	// Sign up to receive TimerEvent
	/**/	vtkSmartPointer<vtkTimerCallback2> cbCircleLune = vtkSmartPointer<vtkTimerCallback2>::New();
	/**/	cbCircleLune->setAngleSoitMeme(0);
	/**/	cbCircleLune->setType(11);
	/**/	cbCircleLune->actor = actorcLune;
	/*********************************************	Fin OBJECT 10: Lune	***************************************************/

/***********************************************	OBJECT 5: Mars	*****************************************************/
/**/	// Creation du Mars 
	/**/	actor5 = PLanet("Assets/Textures/Mars.jpg", 0.7, -7.60511 - RadiusSoleil);
	/**/	// Sign up to receive TimerEvent
	/**/	vtkSmartPointer<vtkTimerCallback2> cb5 = vtkSmartPointer<vtkTimerCallback2>::New();
	/**/	cb5->setAngleSoitMeme(0);
	/**/	cb5->setType(5);
	/**/	cb5->actor = actor5;
	/**/
	/**/	/************************************	Orbite de Mars	******************************************/
	/**/	// Create a circle
	/**/	vtkSmartPointer<vtkRegularPolygonSource> Circle5 = vtkSmartPointer<vtkRegularPolygonSource>::New();
	/**/
	/**/	//polygonSource->GeneratePolygonOff(); // Uncomment this line to generate only the outline of the circle
	/**/	Circle5->SetNumberOfSides(50);
	/**/	Circle5->SetRadius(7.60511 + RadiusSoleil);
	/**/	Circle5->SetCenter(0, 0, 0);
	/**/
	/**/	// Create a mapper and actor
	/**/	vtkSmartPointer<vtkPolyDataMapper> mapperc5 = vtkSmartPointer<vtkPolyDataMapper>::New();
	/**/	mapperc5->SetInputConnection(Circle5->GetOutputPort());
	/**/	actorc5->SetMapper(mapperc5);
	/**/	actorc5->GetProperty()->SetColor(0, 255, 250);
	/**/	actorc5->GetProperty()->SetRepresentationToWireframe();
	/**/
	/*********************************************	Fin OBJECT 5: Mars	***************************************************/

	/***********************************************	OBJECT 6: Jupiter	*****************************************************/
	/**/	// Creation du Jupiter 
	/**/	actor6 = PLanet("Assets/Textures/Jupiter.jpg", 0.7, -15.978865 - RadiusSoleil);
	/**/	// Sign up to receive TimerEvent
	/**/	vtkSmartPointer<vtkTimerCallback2> cb6 = vtkSmartPointer<vtkTimerCallback2>::New();
	/**/	cb6->setAngleSoitMeme(0);
	/**/	cb6->setType(6);
	/**/	cb6->actor = actor6;
	/**/
	/**/	/************************************	Orbite de Jupiter	******************************************/
	/**/	// Create a circle
	/**/	vtkSmartPointer<vtkRegularPolygonSource> Circle6 = vtkSmartPointer<vtkRegularPolygonSource>::New();
	/**/
	/**/	//polygonSource->GeneratePolygonOff(); // Uncomment this line to generate only the outline of the circle
	/**/	Circle6->SetNumberOfSides(50);
	/**/	Circle6->SetRadius(15.978865 + RadiusSoleil);
	/**/	Circle6->SetCenter(0, 0, 0);
	/**/
	/**/	// Create a mapper and actor
	/**/	vtkSmartPointer<vtkPolyDataMapper> mapperc6 = vtkSmartPointer<vtkPolyDataMapper>::New();
	/**/	mapperc6->SetInputConnection(Circle6->GetOutputPort());
	/**/	actorc6->SetMapper(mapperc6);
	/**/	actorc6->GetProperty()->SetColor(0, 255, 250);
	/**/	actorc6->GetProperty()->SetRepresentationToWireframe();
	/**/
	/*********************************************	Fin OBJECT 6: Jupiter	***************************************************/


	/***********************************************	OBJECT 7: Saturne	*****************************************************/
	/**/	// Creation du Saturne 
	/**/	actor7 = PLanet("Assets/Textures/Saturne.jpg", 0.7, -20.853 - RadiusSoleil);
	/**/	// Sign up to receive TimerEvent
	/**/	vtkSmartPointer<vtkTimerCallback2> cb7 = vtkSmartPointer<vtkTimerCallback2>::New();
	/**/	cb7->setAngleSoitMeme(0);
	/**/	cb7->setType(7);
	/**/	cb7->actor = actor7;
	/**/
	/**/	/************************************	Orbite de Saturne	******************************************/
	/**/	// Create a circle
	/**/	vtkSmartPointer<vtkRegularPolygonSource> Circle7 = vtkSmartPointer<vtkRegularPolygonSource>::New();
	/**/
	/**/	//polygonSource->GeneratePolygonOff(); // Uncomment this line to generate only the outline of the circle
	/**/	Circle7->SetNumberOfSides(50);
	/**/	Circle7->SetRadius(20.853 + RadiusSoleil);
	/**/	Circle7->SetCenter(0, 0, 0);
	/**/
	/**/	// Create a mapper and actor
	/**/	vtkSmartPointer<vtkPolyDataMapper> mapperc7 = vtkSmartPointer<vtkPolyDataMapper>::New();
	/**/	mapperc7->SetInputConnection(Circle7->GetOutputPort());
	/**/	actorc7->SetMapper(mapperc7);
	/**/	actorc7->GetProperty()->SetColor(0, 255, 250);
	/**/	actorc7->GetProperty()->SetRepresentationToWireframe();
	/**/
	/*********************************************	Fin OBJECT 7: Saturne	***************************************************/

	/***********************************************	OBJECT 8: Uranus	*****************************************************/
	/**/	// Creation du Uranus 
	/**/	actor8 = PLanet("Assets/Textures/Uranus.jpg", 0.7, -25.8 - RadiusSoleil);
	/**/	// Sign up to receive TimerEvent
	/**/	vtkSmartPointer<vtkTimerCallback2> cb8 = vtkSmartPointer<vtkTimerCallback2>::New();
	/**/	cb8->setAngleSoitMeme(0);
	/**/	cb8->setType(8);
	/**/	cb8->actor = actor8;
	/**/
	/**/	/************************************	Orbite de Uranus	******************************************/
	/**/	// Create a circle
	/**/	vtkSmartPointer<vtkRegularPolygonSource> Circle8 = vtkSmartPointer<vtkRegularPolygonSource>::New();
	/**/
	/**/	//polygonSource->GeneratePolygonOff(); // Uncomment this line to generate only the outline of the circle
	/**/	Circle8->SetNumberOfSides(50);
	/**/	Circle8->SetRadius(25.8 + RadiusSoleil);
	/**/	Circle8->SetCenter(0, 0, 0);
	/**/
	/**/	// Create a mapper and actor
	/**/	vtkSmartPointer<vtkPolyDataMapper> mapperc8 = vtkSmartPointer<vtkPolyDataMapper>::New();
	/**/	mapperc8->SetInputConnection(Circle8->GetOutputPort());
	/**/	actorc8->SetMapper(mapperc8);
	/**/	actorc8->GetProperty()->SetColor(0, 255, 250);
	/**/	actorc8->GetProperty()->SetRepresentationToWireframe();
	/**/
	/*********************************************	Fin OBJECT 8: Uranus	***************************************************/


	/***********************************************	OBJECT 9: Neptune	*****************************************************/
	/**/	// Creation du Neptune 
	/**/	actor9 = PLanet("Assets/Textures/Neptune.jpg", 0.7, -30 - RadiusSoleil);
	/**/	// Sign up to receive TimerEvent
	/**/	vtkSmartPointer<vtkTimerCallback2> cb9 = vtkSmartPointer<vtkTimerCallback2>::New();
	/**/	cb9->setAngleSoitMeme(0);
	/**/	cb9->setType(9);
	/**/	cb9->actor = actor9;
	/**/
	/**/	/************************************	Orbite de Neptune	******************************************/
	/**/	// Create a circle
	/**/	vtkSmartPointer<vtkRegularPolygonSource> Circle9 = vtkSmartPointer<vtkRegularPolygonSource>::New();
	/**/
	/**/	//polygonSource->GeneratePolygonOff(); // Uncomment this line to generate only the outline of the circle
	/**/	Circle9->SetNumberOfSides(50);
	/**/	Circle9->SetRadius(30 + RadiusSoleil);
	/**/	Circle9->SetCenter(0, 0, 0);
	/**/
	/**/	// Create a mapper and actor
	/**/	vtkSmartPointer<vtkPolyDataMapper> mapperc9 = vtkSmartPointer<vtkPolyDataMapper>::New();
	/**/	mapperc9->SetInputConnection(Circle9->GetOutputPort());
	/**/	actorc9->SetMapper(mapperc9);
	/**/	actorc9->GetProperty()->SetColor(0, 255, 250);
	/**/	actorc9->GetProperty()->SetRepresentationToWireframe();
	/**/
	/*********************************************	Fin OBJECT 9: Neptune	***************************************************/
	//vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	//vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	renderWindowInteractor->SetRenderWindow(renderWindow);

	/******************************BACKGROUND IMAGE ********************/
	vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
	vtkSmartPointer<vtkJPEGReader> jpegReader = vtkSmartPointer<vtkJPEGReader>::New();

	jpegReader->SetFileName("Assets/Textures/galaxyWallpaper.jpg");
	jpegReader->Update();
	imageData = jpegReader->GetOutput();
	cout << jpegReader->GetFileName() << endl;

	// creer un actor au backgroud 
	vtkSmartPointer<vtkImageActor> imageActor = vtkSmartPointer<vtkImageActor>::New();

	imageActor->SetInputData(imageData);

	// Create a renderer to display the image in the background
	vtkSmartPointer<vtkRenderer> backgroundRenderer = vtkSmartPointer<vtkRenderer>::New();

	// Create a superquadric
	vtkSmartPointer<vtkSuperquadricSource> superquadricSource = vtkSmartPointer<vtkSuperquadricSource>::New();
	superquadricSource->SetPhiRoundness(1.1);
	superquadricSource->SetThetaRoundness(.2);

	/*************************************************END BACKGROUND DEFINITION **********************************************/


	// Set up the render window and renderers such that there is
	// a background layer and a foreground layer
	backgroundRenderer->SetLayer(0);
	backgroundRenderer->InteractiveOff();
	renderer->SetLayer(1);
	renderWindow->SetNumberOfLayers(2);
	renderWindow->AddRenderer(backgroundRenderer);
	renderWindow->AddRenderer(renderer);
	// Camera
	camera->SetPosition(0, 0, 120);
	camera->SetFocalPoint(0, 0, 0);
	renderer->SetActiveCamera(camera);

	// Add the actor to the scene
	renderer->AddActor(actor1);
	renderer->AddActor(actor2);
	renderer->AddActor(actorc2);
	renderer->AddActor(actor3);
	renderer->AddActor(actorc3);
	renderer->AddActor(actor4);
	renderer->AddActor(actorc4);
	renderer->AddActor(actorLune);
	renderer->AddActor(actorcLune);
	renderer->AddActor(actor5);
	renderer->AddActor(actorc5);
	renderer->AddActor(actor6);
	renderer->AddActor(actorc6);
	renderer->AddActor(actor7);
	renderer->AddActor(actorc7);
	renderer->AddActor(actor8);
	renderer->AddActor(actorc8);
	renderer->AddActor(actor9);
	renderer->AddActor(actorc9);


	/**********************************Set CAMERA TO THE BACKGROUND **********************************************/
	backgroundRenderer->AddActor(imageActor);
	//	renderer->SetBackground(1, 1, 1); // Background color white
		// Set up the background camera to fill the renderer with the image
	double origin[3];
	double spacing[3];
	int extent[6];
	imageData->GetOrigin(origin);
	imageData->GetSpacing(spacing);
	imageData->GetExtent(extent);

	vtkSmartPointer<vtkCamera> cam = vtkSmartPointer<vtkCamera>::New();
	cam = backgroundRenderer->GetActiveCamera();
	cam->ParallelProjectionOn();

	double xc = origin[0] + 0.5 * (extent[0] + extent[1]) * spacing[0];
	double yc = origin[1] + 0.5 * (extent[2] + extent[3]) * spacing[1];
	//double xd = (extent[1] - extent[0] + 1)*spacing[0];
	double yd = (extent[3] - extent[2] + 1) * spacing[1];
	double d = camera->GetDistance();
	cam->SetParallelScale(0.4 * yd);
	cam->SetFocalPoint(xc, yc, 0.0);
	cam->SetPosition(xc, yc, d);


	/*****************************************************Fin background camera *****************************************/

	// Render and interact
	renderWindow->SetFullScreen(true);

	renderWindow->Render();

	// Initialize must be called prior to creating timer events.
	renderWindowInteractor->Initialize();


	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb1);
	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb2);
	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb3);
	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb4);
	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cbLune);
	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cbCircleLune);
	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb5);
	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb6);
	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb7);
	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb8);
	renderWindowInteractor->AddObserver(vtkCommand::TimerEvent, cb9);


	vtkSmartPointer<KeyPressInteractorStyle> style = vtkSmartPointer<KeyPressInteractorStyle>::New();
	renderWindowInteractor->SetInteractorStyle(style);
	style->SetCurrentRenderer(renderer);

	int timerId = renderWindowInteractor->CreateRepeatingTimer(1);
	std::cout << "timerId: " << timerId << std::endl;

	play_music_bg();
	// Start the interaction and timer
	renderWindowInteractor->Start();
	return EXIT_SUCCESS;
}


/**********************************************************************************************************************************/

vtkSmartPointer<vtkActor> PLanet(const char* imagepath, float size, float PositionX) {

	// Create a sphere with texture coordinates
	vtkSmartPointer<vtkTexturedSphereSource> source = vtkSmartPointer<vtkTexturedSphereSource>::New();
	source->SetThetaResolution(20);
	source->SetPhiResolution(20);
	source->SetRadius(size);
	source->Update();
	myPlanets.push_back(source);
	cout << "  Rashid" << endl;

	// Read texture file
	vtkSmartPointer<vtkImageReader2Factory> readerFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
	vtkImageReader2* imageReader = readerFactory->CreateImageReader2(imagepath);
	imageReader->SetFileName(imagepath);
	// Create texture
	vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();
	texture->SetInputConnection(imageReader->GetOutputPort());

	vtkSmartPointer<vtkTransformTextureCoords> transformTexture = vtkSmartPointer<vtkTransformTextureCoords>::New();
	transformTexture->SetInputConnection(source->GetOutputPort());
	transformTexture->SetPosition(0, 0, 0);


	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(transformTexture->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->SetPosition(PositionX, 0, 0);
	actor->SetTexture(texture);

	return actor;
}

vtkSmartPointer<vtkActor> PLanet(const char* imagepath, float size, float PositionX, float PositionY, float PositionZ) {

	// Create a sphere with texture coordinates
	vtkSmartPointer<vtkTexturedSphereSource> source = vtkSmartPointer<vtkTexturedSphereSource>::New();
	source->SetThetaResolution(20);
	source->SetPhiResolution(20);
	source->SetRadius(size);
	source->Update();
	myPlanets.push_back(source);
	cout << "  Rashid" << endl;

	// Read texture file
	vtkSmartPointer<vtkImageReader2Factory> readerFactory = vtkSmartPointer<vtkImageReader2Factory>::New();
	vtkImageReader2* imageReader = readerFactory->CreateImageReader2(imagepath);
	imageReader->SetFileName(imagepath);
	// Create texture
	vtkSmartPointer<vtkTexture> texture = vtkSmartPointer<vtkTexture>::New();
	texture->SetInputConnection(imageReader->GetOutputPort());

	vtkSmartPointer<vtkTransformTextureCoords> transformTexture = vtkSmartPointer<vtkTransformTextureCoords>::New();
	transformTexture->SetInputConnection(source->GetOutputPort());
	transformTexture->SetPosition(0, 0, 0);


	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(transformTexture->GetOutputPort());

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	actor->SetPosition(PositionX, PositionY, PositionZ);
	actor->SetTexture(texture);

	return actor;
}

void Cam(vtkActor* actor, vtkCamera* camera, bool Soleil) {
	vtkMatrix4x4* m;			  // Représente la matrice de Trasnformation
	double Tab[3];				  // Représente le Point de départ
	double Result[3];			  // Représente le Point d'arrivée ( aprés transformation)
	actor->GetPosition(Tab);     // Représente le Point de départ
	m = actor->GetUserMatrix();  // Représente la matrice de Trasnformation
	if (Soleil) {
		// deplacer la caméra
		camera->SetPosition(0, 0, 0 + 120);
		camera->SetFocalPoint(0, 0, 0);
	}
	else {
		Result[0] = m->GetElement(0, 0) * Tab[0] + m->GetElement(0, 1) * Tab[1] + m->GetElement(0, 2) * Tab[2] + m->GetElement(0, 3);
		Result[1] = m->GetElement(1, 0) * Tab[0] + m->GetElement(1, 1) * Tab[1] + m->GetElement(1, 2) * Tab[2] + m->GetElement(1, 3);
		Result[2] = m->GetElement(2, 0) * Tab[0] + m->GetElement(2, 1) * Tab[1] + m->GetElement(2, 2) * Tab[2] + m->GetElement(2, 3);
		// deplacer la caméra
		camera->SetPosition(Result[0], Result[1], Result[2] + 60);
		camera->SetFocalPoint(Result);
	}

}






