///////////////////////////////////////////////////////////////////
//                                                               //
//            AddTaskZNZP Macro to run on grids               //
//                                                               //
//                                                               //
///////////////////////////////////////////////////////////////////
class AliAnalysisDataContainer;
#include "AliAnalysisTaskZNZP.h"

AliAnalysisTaskZNZP* AddTaskZNZP(const char* taskname = "ZNZP",
                                 const char* suffix = "") {
  AliAnalysisManager* mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    return 0x0;
  }
  // get the input event handler this handler is part of the managing system and
  // feeds events to your task
  if (!mgr->GetInputEventHandler()) {
    return 0x0;
  }

  AliAnalysisTaskZNZP* taskKno = new AliAnalysisTaskZNZP("taskKno");
  if (!taskKno) {
    return 0x0;
  }
  taskKno->SetUseMC(false);
  taskKno->SetPeriod("lhc18r");
  taskKno->SetV0Mmin(0.);
  taskKno->SetV0Mmax(80.);
  taskKno->SetEtaCut(0.8);
  taskKno->SetPtCut(0.15);
  taskKno->IsTowerEnergy(true);
  taskKno->SetTrigger(AliVEvent::kINT7);
  mgr->AddTask(taskKno);

  mgr->ConnectInput(taskKno, 0, mgr->GetCommonInputContainer());
  mgr->ConnectOutput(
      taskKno, 1,
      mgr->CreateContainer(
          Form("cList%s_%s", taskname, suffix), TList::Class(),
          AliAnalysisManager::kOutputContainer,
          Form("%s:%s", AliAnalysisManager::GetCommonFileName(), taskname)));
  return taskKno;
}
