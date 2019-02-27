#pragma once

#define DECLARE_SINGLE_PROGRAM_RUN(program_name) \
class SingleProgramRun \
{\
  static CMutex m_RunOnceMutex;\
  static CSingleLock m_RunOnceLock;\
public:\
  static bool CheckIsAlreadyRunning(){return !m_RunOnceLock.Lock(0);}\
  static bool ShowPreviousInstance(CString const& caption){\
        HWND w = FindWindow(0, caption);\
        if (w)\
        {\
            ShowWindow(w, SW_SHOW);\
            SetActiveWindow(w);\
            SetForegroundWindow(w);\
            return true;\
        }\
        return false; \
;}\
};\
CMutex SingleProgramRun::m_RunOnceMutex(FALSE, _T("AlreadyRunning##program_name")); \
CSingleLock SingleProgramRun::m_RunOnceLock(&SingleProgramRun::m_RunOnceMutex); 


#define CHECK_SINGLE_PROGRAM_RUN_AND_EXIT(program_caption) \
/*проверяем, не запущена ли другая копия*/ \
if (!SingleProgramRun::CheckIsAlreadyRunning())\
    /*запущена - сообщяем и активируем*/\
    if (ShowPreviousInstance(program_caption)) \
          exit(0);

