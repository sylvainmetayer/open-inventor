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

void mySelectionCB(void *, SoPath *selectionPath)
{
	char tmpStr[256];
	float x = 3, z = 5;
	sprintf(tmpStr, "%f, %f", x, z);
}


SoPath *pickFilterCB(void *, const SoPickedPoint *pick)
{
	// See which child got picked
	SoPath *p = pick->getPath();
	SbVec3f v = pick->getPoint();
	int i;
	for (i = 0; i < p->getLength() - 1; i++)
	{
		SoNode *n = p->getNode(i);
		if (n->isOfType(SoSelection::getClassTypeId()))
			break;
	}
	return p->copy(i, 2);
}

int
main(int, char **argv)
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
  SoSeparator *separator1, *separator2, *separator3;

  translation->translation.setValue(0,7.5,0);
  rotationXYZ->angle = M_PI/2;
  rotationXYZ->angle = SoRotationXYZ::X;
  translation2->translation.setValue(0, 6.5, 0);
  
  separator1 = new SoSeparator();
  separator2 = new SoSeparator();
  separator3 = new SoSeparator();

  root->ref();

  SoTranslation *crayonTranslation = new SoTranslation();
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

  crayonTranslation->translation.connectFrom(&calXZ->oA);

  texture->filename.setValue("C:/Open Inventor/9.4.1 C++ Visual2013 x64/src/FXViz/data/metal.bmp");
  
  separator2->addChild(texture);
  separator2->addChild(crayon);
  
  separator1->addChild(translation2);
  separator1->addChild(rotationXYZ);
  texture2->filename.setValue("C:/Open Inventor/9.4.1 C++ Visual2013 x64/src/FXViz/data/oak.gif");
  separator1->addChild(texture2);
  separator1->addChild(pointe);
  separator1->addChild(material);

  root->addChild(crayonTranslation);
  root->addChild(separator2);
  root->addChild(separator3);
  root->addChild(separator1);

  separator3->addChild(translation);
  separator3->addChild(mine);

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