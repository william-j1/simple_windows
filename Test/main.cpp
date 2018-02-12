#include <Sw.h>
#include <Sw\Window.h>
#include <Sw\Label.h>
#include <Sw\Button.h>
#include <Sw\TextBox.h>
#include <Sw\MDIClient.h>
#include <Sw\MDIFrame.h>
#include <Sw\MDIChild.h>
#include <Sw\Menu.h>
#include <Windows.h>

using namespace Sw;

void WindowTest()
{
	Window *pWindow = Window::Create(CW_USEDEFAULT, CW_USEDEFAULT, 300, 200, "Hello World", "MyClass");
	pWindow->Show();

	Label *pLabel = Label::Create(pWindow, 10, 10, 100, 100, "TESTING");
	pLabel->SetText("FOOBAR");

	Button *pButton = Button::Create(pWindow, 10, 30, 100, 20, "Test Button 1");
	pButton->OnClickEvent([=](){
		MessageBox(pWindow->GetHWnd(), "Button 1 clicked", "Test", MB_OK);
	});

	Button *pButton2 = Button::Create(pWindow, 10, 50, 100, 20, "Test Button 2");
	pButton2->OnClickEvent([=](){
		MessageBox(pWindow->GetHWnd(), "Button 2 clicked", "Test", MB_OK);
	});

	TextBox *pTextBox1 = Sw::TextBox::Create(pWindow, 10, 70, 200, 20, "Default Text");
	pTextBox1->SetText("I like dynamic components");
}

void MDITest()
{
	MDIFrame *pMIDIFrame = MDIFrame::Create(CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, "MDI Test", "MDIWindow1");
	pMIDIFrame->Show();

	Menu *pMenuBar = Menu::Create(pMIDIFrame);	
	Menu *pMenuFile = Menu::Create(true);
	pMenuFile->InsertText("Test 1");
	pMenuFile->InsertText("Test 2");
	pMenuFile->InsertText("Test 3");
	pMenuBar->InsertPopup("File", pMenuFile);

	MDIChild *pMDIChild = MDIChild::Create(pMIDIFrame, 20, 20, 300, 200, "Hello", "MDIChild1");
	pMDIChild->Show();

	Label *pLabel = Label::Create(pMDIChild, 10, 10, 100, 100, "TESTING");
	pLabel->SetText("FOOBAR");

	Button *pButton = Button::Create(pMDIChild, 10, 30, 100, 20, "Test Button 1");
	pButton->OnClickEvent([=](){
		MessageBox(pMDIChild->GetHWnd(), "Button 1 clicked", "Test", MB_OK);
	});

	Button *pButton2 = Button::Create(pMDIChild, 10, 50, 100, 20, "Test Button 2");
	pButton2->OnClickEvent([=](){
		MessageBox(pMDIChild->GetHWnd(), "Button 2 clicked", "Test", MB_OK);
	});

	TextBox *pTextBox1 = Sw::TextBox::Create(pMDIChild, 10, 70, 200, 20, "Default Text");
	pTextBox1->SetText("I like dynamic components");
}

int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{	
	Startup();

	WindowTest();
	MDITest();

	int nExitCode = Sw::Pump();

	Cleanup();

	return nExitCode;
}
