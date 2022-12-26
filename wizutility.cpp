#include "wizutility.h"
#include "ui_wizutility.h"
#include "proc.h"
#include <iostream>
#include <QTimer>

// Global Vars
HANDLE hProcess;
uintptr_t moduleBase;
    // CHANGE THESE VALUES UPON GAME UPDATE
uintptr_t playerCoordBaseAddr, questCoordBaseAddr, yAddy, yQuestAddy;
const std::vector<unsigned int> playerYOffsets = { 0x8 };
const std::vector<unsigned int> questYOffsets = { 0x8, 0x30, 0x48, 0x628, 0x248, 0xD04 };

WizUtility::WizUtility(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::WizUtility)
{
    ui->setupUi(this);
    // Start timer for coordinate update
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateCoords()));
    timer->start(1000);
}

WizUtility::~WizUtility()
{
    delete ui;
}

void WizUtility::updateCoords()
{
    // Print current coords
    float yCurrent = 404, zCurrent = 404, xCurrent = 404;
    ReadProcessMemory(hProcess, (BYTE*)yAddy, &yCurrent, sizeof(yCurrent), nullptr);
    ReadProcessMemory(hProcess, (BYTE*)(yAddy - 4), &zCurrent, sizeof(zCurrent), nullptr);
    ReadProcessMemory(hProcess, (BYTE*)(yAddy - 8), &xCurrent, sizeof(xCurrent), nullptr);
    ui->xCoordEdit->setPlainText(QString::number(xCurrent));
    ui->yCoordEdit->setPlainText(QString::number(yCurrent));
    ui->zCoordEdit->setPlainText(QString::number(zCurrent));
}

void WizUtility::on_attachButton_clicked()
{
    // Get Process ID of Wizard101
    DWORD procId = GetProcId(L"WizardGraphicalClient.exe");

    // Get Wizard101 module Base Address
    moduleBase = GetModuleBaseAddress(procId, L"WizardGraphicalClient.exe");

    // Get a Handle to the Process
    hProcess = 0;
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, procId);

    // Debug
    std::cout << "Process ID = " << procId << std::endl;
    std::cout << "ModuleBase = " << "0x" << std::hex << moduleBase << std::endl;

    // Resolve Player and Quest y Coordinate Addresses
    playerCoordBaseAddr = moduleBase + 0x03243438;
    yAddy = FindDMAAddy(hProcess, playerCoordBaseAddr, playerYOffsets);
    questCoordBaseAddr = moduleBase + 0x031C3C18;
    yQuestAddy = FindDMAAddy(hProcess, questCoordBaseAddr, questYOffsets);
}


void WizUtility::on_questPortButton_clicked()
{
    // Write quest coordinate values to respective player coordinate addresses
    float yQuest = 404, zQuest = 404, xQuest = 404;
    ReadProcessMemory(hProcess, (BYTE*)yQuestAddy, &yQuest, sizeof(yQuest), nullptr);
    ReadProcessMemory(hProcess, (BYTE*)(yQuestAddy - 4), &zQuest, sizeof(zQuest), nullptr);
    ReadProcessMemory(hProcess, (BYTE*)(yQuestAddy - 8), &xQuest, sizeof(xQuest), nullptr);
    std::cout << "Setting player coords to quest coords..." << std::endl;
    const int zOffset = 20;
    zQuest += zOffset;
    WriteProcessMemory(hProcess, (BYTE*)yAddy, &yQuest, sizeof(yQuest), nullptr);
    WriteProcessMemory(hProcess, (BYTE*)(yAddy - 4), &zQuest, sizeof(zQuest), nullptr);
    WriteProcessMemory(hProcess, (BYTE*)(yAddy - 8), &xQuest, sizeof(xQuest), nullptr);
}

