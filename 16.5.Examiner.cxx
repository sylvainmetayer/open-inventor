#include <Inventor/SoMacApp.h>
#include <Inventor/SoDB.h>         
#include <Inventor/Xt/SoXt.h>         
#include <Inventor/Xt/viewers/SoXtExaminerViewer.h>  
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor\nodes\SoCylinder.h>
#include <Inventor\nodes/SoTranslation.h>
#include <Inventor\nodes/SoRotationXYZ.h>
#include <Inventor\nodes/SoTexture2.h>
#include <Inventor\nodes/SoMaterial.h>
#include <Inventor\nodes\SoText3.h>
#include <Inventor\nodes\SoTransform.h>
#include <Inventor\engines\SoCalculator.h>
#include <Inventor\engines\SoTimeCounter.h>
#include <Inventor\SoPath.h>
#include <Inventor\SoPickedPoint.h>
#include <Inventor\SbVec.h>
#include <Inventor\nodes/SoSelection.h>
#include <Inventor\actions/SoBoxHighlightRenderAction.h>
#include <Inventor\sensors\SoFieldSensor.h>
#include <Inventor\nodes\SoCoordinate3.h>
#include <Inventor\nodes\SoLineSet.h>

SoText3 *texte;
char tmpStr[256];
SoTranslation *translationRoot;

void mySelectionCB(void *data, SoSensor *sensor)
{
	SoDataSensor *mySensor = (SoDataSensor *)sensor;
	sprintf(tmpStr, "%f, %f", translationRoot->translation.getValue()[0], translationRoot->translation.getValue()[2]);
	texte->string.setValue(tmpStr);
}

int main(int, char **argv)
{
  // Initialize Inventor and Xt
  Widget myWindow = SoXt::init(argv[0]);
  
  // Build the viewer in the applications main window
  SoXtExaminerViewer *myViewer = 
    new SoXtExaminerViewer(myWindow);

  SoCone *pointe = new SoCone();
  SoCylinder *crayon = new SoCylinder();
  SoCylinder *mine = new SoCylinder();

  crayon->height = 10;
  crayon->radius = 1;
  mine->height = 1;
  mine->radius = 0.1;
  pointe->height = 3;
  pointe->bottomRadius = 1;

  SoTranslation *translation = new SoTranslation();
  SoTranslation *translation2 = new SoTranslation();
  SoRotationXYZ *rotationXYZ = new SoRotationXYZ();

  SoText3 *text3 = new SoText3();
  
  SoTexture2 *texture = new SoTexture2();
  SoTexture2 *texture2 = new SoTexture2();
  SoMaterial *material = new SoMaterial();
  
  SoSeparator *root = new SoSeparator();
  SoSeparator *separatorPointe, *separatorCrayon, *separatorMine;

  translation->translation.setValue(0,7.5,0);
  rotationXYZ->angle = M_PI/2;
  rotationXYZ->angle = SoRotationXYZ::X;
  translation2->translation.setValue(0, 6.5, 0);
  
  separatorPointe = new SoSeparator();
  separatorCrayon = new SoSeparator();
  separatorMine = new SoSeparator();

  root->ref();

  translationRoot = new SoTranslation();
  SoCalculator *calXZ = new SoCalculator();
  SoTimeCounter *timeCounter = new SoTimeCounter();
  timeCounter->max = 360;
  timeCounter->step = 4;
  timeCounter->frequency = 0.075;
  calXZ->a.connectFrom(&timeCounter->output);
  calXZ->expression.set1Value(0, "ta=a*M_PI/180"); // theta
  calXZ->expression.set1Value(1, "tb=5*cos(5*ta)"); // r
  calXZ->expression.set1Value(2, "td=tb*cos(ta)"); //x 
  calXZ->expression.set1Value(3, "te=tb*sin(ta)"); // z
  calXZ->expression.set1Value(4, "oA=vec3f(td,0,te)");

  translationRoot->translation.connectFrom(&calXZ->oA);

  texture->filename.setValue("C:/Open Inventor/9.4.1 C++ Visual2013 x64/src/FXViz/data/metal.bmp");
  
  separatorCrayon->addChild(texture);
  separatorCrayon->addChild(crayon);
  
  separatorPointe->addChild(translation2);
  separatorPointe->addChild(rotationXYZ);
  texture2->filename.setValue("C:/Open Inventor/9.4.1 C++ Visual2013 x64/src/FXViz/data/oak.gif");
  separatorPointe->addChild(texture2);
  separatorPointe->addChild(pointe);
  separatorPointe->addChild(material);

  separatorMine->addChild(translation);
  separatorMine->addChild(mine);

  SoRotationXYZ *rotationRoot = new SoRotationXYZ();
  rotationRoot->angle = M_PI / 2;
  rotationRoot->angle = SoRotationXYZ::X;

  root->addChild(translationRoot);
  root->addChild(rotationRoot);
  root->addChild(separatorCrayon);
  root->addChild(separatorMine);
  root->addChild(separatorPointe);

  SoFieldSensor *mySensor = new SoFieldSensor();
  mySensor->setFunction(mySelectionCB);
  mySensor->attach(&(translationRoot->translation));

  SoCoordinate3 *coordonnees = new SoCoordinate3();
  SoLineSet *lineSet = new SoLineSet();

  // Texte
  texte = new SoText3();
  texte->string.setValue("Du texte");
  root->addChild(texte);

  // Attach the viewer to the scene graph
  myViewer->setSceneGraph(root);
  
  // Show the main window
  myViewer->show();
  SoXt::show(myWindow);
  
  // Loop forever
  SoXt::mainLoop();
  delete myViewer;
  SoXt::finish();  
  
  return 0;
}