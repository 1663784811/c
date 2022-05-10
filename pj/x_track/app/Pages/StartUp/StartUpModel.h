#ifndef __START_UP_MODEL_H
#define __START_UP_MODEL_H

#include "Common/DataProc/DataProc.h"

namespace Page
{
    class StartUpModel
    {
        public:
            void Init();
            void Deint();
            void PlayMusic(const char *music);
            void SetEncoderEnable(bool en)
            {

            }

        private:
            Account *account;
    };
}

#endif
