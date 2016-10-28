//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
USEFORM("commands.cpp", fmMain);
USEFORM("ChooseFile.cpp", fmChooseFile);
USEFORM("..\BuilderForms\TextEdit.cpp", fmTextEdit);
USEFORM("..\BuilderForms\Menu frames and forms\ParInput.cpp", frPar); /* TFrame: File Type */
USEFORM("..\BuilderForms\Menu frames and forms\fmPar.cpp", fmPr);
USEFORM("..\BuilderForms\Menu frames and forms\ListInput.cpp", frList); /* TFrame: File Type */
USEFORM("..\BuilderForms\Menu frames and forms\FitMenu.cpp", fmFitMenu);
USEFORM("..\BuilderForms\Menu frames and forms\fMinMax.cpp", fmMinMax);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "Analysis of FPAP experiments";
                 Application->CreateForm(__classid(TfmMain), &fmMain);
		Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        catch (...)
        {
                 try
                 {
                         throw Exception("");
                 }
                 catch (Exception &exception)
                 {
                         Application->ShowException(&exception);
                 }
        }
        return 0;
}
//---------------------------------------------------------------------------
