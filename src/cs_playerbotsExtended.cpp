#include "cs_playerbotsExtended.h"
#include "BattleGroundTactics.h"
#include "Chat.h"
#include "GuildTaskMgr.h"
#include "PerformanceMonitor.h"
#include "PlayerbotMgr.h"
#include "PlayerbotAIConfig.h"
#include "RandomPlayerbotMgr.h"
#include "RandomPlayerbotFactory.h"
#include "ScriptMgr.h"

using namespace Acore::ChatCommands;

//add new commands:
//.playerbotx createrandombot
//.playerbotx setmaxrandombots <count>

class playerbotsExtended_commandscript : public CommandScript
{
public:
    playerbotsExtended_commandscript() : CommandScript("playerbotsExtended_commandscript") { }

    ChatCommandTable GetCommands() const override
    {
        static ChatCommandTable botExtendedCommandTable = {
            {"createrandombot", HandleCreateRandomBot, SEC_GAMEMASTER, Console::Yes},
            {"setmaxrandombots", HandleSetMaxRandomBots, SEC_ADMINISTRATOR, Console::Yes}
        };

        static ChatCommandTable playerbotsExtendedTable = {
            {"playerbotx", botExtendedCommandTable}
        };

        return playerbotsExtendedTable;
    }

    // .playerbotx createrandombot
    static bool HandleCreateRandomBot(ChatHandler* handler, const char* /*args*/)
    {

        RandomPlayerbotFactory::CreateRandomBots(); // fills up to max

        handler->PSendSysMessage("New Random Player Bots Created");

        return true;
    }

    // .playerbotx setmaxrandombots <count>
    static bool HandleSetMaxRandomBots(ChatHandler* handler, const char* args)
    {
        if (!args || !*args)
        {
            handler->PSendSysMessage("Usage: .playerbotx setmaxrandombots <number>");
            return false;
        }

        // Convert input safely
        char* endPtr = nullptr;
        unsigned long count = strtoul(args, &endPtr, 10);

        // Check for invalid input or overflow
        if (*endPtr != '\0' || count > UINT32_MAX)
        {
            handler->PSendSysMessage("Invalid number: %s", args);
            return false;
        }

        sPlayerbotAIConfig->maxRandomBots = static_cast<int32>(count);
        handler->PSendSysMessage("Max random bot count set to %u.", static_cast<int32>(count));

        return true;
    }

};

void AddSC_playerbotsExtended_commandscript()
{
    new playerbotsExtended_commandscript();
}