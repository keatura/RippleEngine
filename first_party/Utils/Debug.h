#pragma once

typedef struct _MonoString MonoString;

class Debug
{
public:
    static void Initialize();
    static void Log(MonoString* mono_string);
};

