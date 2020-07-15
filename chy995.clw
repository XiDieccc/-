; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChy995View
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "chy995.h"
LastPage=0

ClassCount=6
Class1=CChy995App
Class2=CChy995Doc
Class3=CChy995View
Class4=CMainFrame

ResourceCount=3
Resource1=IDD_ABOUTBOX
Class5=CAboutDlg
Resource2=IDR_MAINFRAME
Class6=CHistogramDlg
Resource3=IDD_DIALOG1

[CLS:CChy995App]
Type=0
HeaderFile=chy995.h
ImplementationFile=chy995.cpp
Filter=N

[CLS:CChy995Doc]
Type=0
HeaderFile=chy995Doc.h
ImplementationFile=chy995Doc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CChy995Doc

[CLS:CChy995View]
Type=0
HeaderFile=chy995View.h
ImplementationFile=chy995View.cpp
Filter=C
BaseClass=CScrollView
VirtualFilter=VWC
LastObject=CChy995View


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=ID_IDEALFILTER




[CLS:CAboutDlg]
Type=0
HeaderFile=chy995.cpp
ImplementationFile=chy995.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_APP_ABOUT
CommandCount=13

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_SAVE
Command2=ID_APP_ABOUT
Command3=ID_GRAY
Command4=ID_HISTOGRAM
Command5=ID_LINETRANS
Command6=ID_EQUALIZE
Command7=ID_FOURIER
Command8=ID_IFOURIER
Command9=ID_FFOURIER
Command10=ID_IFFOURIER
Command11=ID_AVERAGEFILTER
Command12=ID_MEDIANFILTER
Command13=ID_GradSharp
Command14=ID_RAPLASSHARPEN
Command15=ID_IDEALFILTER
CommandCount=15

[DLG:IDD_DIALOG1]
Type=1
Class=CHistogramDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816

[CLS:CHistogramDlg]
Type=0
HeaderFile=HistogramDlg.h
ImplementationFile=HistogramDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CHistogramDlg
VirtualFilter=dWC

