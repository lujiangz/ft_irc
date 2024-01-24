#pragma once


//----COMMAND_MESSAGES
#define CAP_LS ":ircserv CAP * LS :"
#define NICK(OldNick, NewNick) ":" + OldNick + " NICK " + NewNick
#define MODE(FromWho, ChanName, ModeStr, Target) ":" + FromWho + " MODE " + ChanName + " " + ModeStr + " " + Target
#define PRIVMSG(FromWho, To, Message) ":" + FromWho + " PRIVMSG " + To + " :" + Message
#define INVITE(FromWho, To, ChanName) ":" + FromWho + " INVITE " + To + " " + ChanName
#define JOIN(Nick, ChanName) ":" + Nick + " JOIN " + ChanName
#define KICK(Nick, ChanName, KickedNick) ":" + Nick + " KICK " + ChanName + " " + KickedNick
#define PART(Nick, ChanName) ":" + Nick + " PART " + ChanName
#define QUIT(Nick, Reason) ":" + Nick + " QUIT :Quit: " + Reason

//----REPLIES
#define RPL_WELCOME(Nick, UserName) ":ircserv 001 " + Nick + " :Welcome to ircserv, " + Nick + "!" + UserName + "" 

#define TOKENS "CASEMAPPING=ascii, CHANLIMIT=#:4, CHANMODES=b,i,k,l,o,t, PREFIX=(o)@, TARGMAX=NAMES:1,LIST:1,KICK:1,JOIN:1,PRIVMSG:1,NOTICE:1,PART:0,QUIT:0, TOPICLEN=254"

#define RPL_ISUPPORT(Nick)  ":ircserv 005 " + Nick + " " + TOKENS + " :are supported by this server"

//RPL_LISTSTART (321) "<client> Channel :Users  Name"

#define RPL_LIST(Nick, ChanName, ChanCount, Topic) ":ircserv 322 " + Nick + " " + ChanName + " " + ChanCount + " :" + Topic

#define RPL_LISTEND(Nick) ":ircserv 323 " + Nick + " " + ":End of /LIST"

#define RPL_CHANNELMODEIS(Nick, ChanName, ModeString) ":ircserv 324 " + Nick + " " + ChanName + " " + ModeString

#define RPL_NOTOPIC(Nick, ChanName) ":ircserv 331 " + Nick + " " + ChanName + " :No topic is set"

#define RPL_TOPIC(Nick, ChanName, Topic) ":ircserv 332 " + Nick + " " + ChanName + " :" + Topic

//#define RPL_TOPICWHOTIME(Nick, ChanName, TopicSetterNick, TimeStamp) ":ircserv 333 " + Nick + " " + ChanName + " " + TopicSetterNick + " " + TimeStamp

//RPL_INVITELIST (336) "<client> <channel>"

//RPL_ENDOFINVITELIST (337) "<client> :End of /INVITE list"

#define RPL_INVITING(Nick, InvitedNick, ChanName) ":ircserv 341 " + Nick + " " + InvitedNick + " " + ChanName

#define RPL_NAMREPLY(Nick, ChanName, PrefixNickList) ":ircserv 353 " + Nick + " = " + ChanName + " :" + PrefixNickList

#define RPL_ENDOFNAMES(Nick, ChanName) ":ircserv 366 " + Nick + " " + ChanName + " :End of /NAMES list"

#define RPL_BANLIST(Nick, ChanName, Mask) ":ircserv 367 " + Nick + " " + ChanName + " " + Mask

#define RPL_ENDOFBANLIST(Nick, ChanName) ":ircserv 368 " + Nick + " " + ChanName + " :End of channel ban list"

//#define RPL_WHOISMODES(Nicki, Modes) ":ircserv 379 " + Nick + " :is using modes " + Modes 

//----ERRORS
#define ERR_UNKNOWNERROR(Nick, Command, Message) ":ircserv 400 " + Nick + " " + Command + " :" + Message

#define ERR_NOSUCHNICK(Nick, Nickname) ":ircserv 401 " + Nick + " " + Nickname + " :No such nick"

#define ERR_NOSUCHCHANNEL(Nick, ChanName) ":ircserv 403 " + Nick + " " + ChanName + " :No such channel"

#define ERR_CANNOTSENDTOCHAN(Nick, ChanName) ":ircserv 404 " + Nick + " " + ChanName +  " :Cannot send to channel"

#define ERR_TOOMANYCHANNELS(Nick, ChanName) ":ircserv 405 " + Nick + " " + ChanName + " :You have joined too many channels"

#define ERR_NORECIPIENT(Nick, Command) ":ircserv 411 " + Nick + " " + ":No recipient given (" + Command + ")"

#define ERR_NOTEXTTOSEND(Nick) ":ircserv 412 " + Nick + " " + ":No text to send"

//#define ERR_INPUTTOOLONG(Nick) ":ircserv 417 " + Nick + " " + ":Input line was too long"

//#define ERR_UNKNOWNCOMMAND(Nick, Command) ":ircserv 421 " + Nick + " " + Command + " :Unknown command"

#define ERR_NONICKNAMEGIVEN(Nick) ":ircserv 431 " + Nick + " " + ":No nickname given"

#define ERR_ERRONEUSNICKNAME(Nick) ":ircserv 432 " + Nick + " " + Nick + " :Erroneus nickname"

#define ERR_NICKNAMEINUSE(Nick) ":ircserv 433 " + Nick + " " + Nick + " :Nickname is already in use"

#define ERR_USERNOTINCHANNEL(Nick, Client, ChanName) ":ircserv 441 " + Nick + " " + Client + " " + ChanName + " :They aren't on that channel"

#define ERR_NOTONCHANNEL(Nick, ChanName) ":ircserv 442 " + Nick + " " + Nick + " " + ChanName + " :You're not on that channel"

#define ERR_USERONCHANNEL(Nick, Client, ChanName) ":ircserv 443 " + Nick + " " + Client + " " + ChanName + " :is already on channel"

#define ERR_NOTREGISTERED(Nick) ":ircserv 451 " + Nick + " " + ":You have not registered"

#define ERR_NEEDMOREPARAMS(Nick, Command) ":ircserv 461 " + Nick + " " + Command +  " :Not enough parameters"

#define ERR_ALREADYREGISTERED(Nick) ":ircserv 462 " + Nick + " " + ":You may not reregister"

#define ERR_PASSWDMISMATCH(Nick) ":ircserv 464 " + Nick + " " +  ":Password incorrect"

//#define ERR_YOUREBANNEDCREEP(Nick) ":ircserv 462 " + Nick + " " +  ":You banned from this server"

#define ERR_CHANNELISFULL(Nick, ChanName) ":ircserv 471 " + Nick + " " + ChanName + " :Cannot join channel (+l)"

#define ERR_UNKNOWNMODE(Nick, ModeChar) ":ircserv 472 " + Nick + " " + ModeChar + " :is unknown mode char to me"

#define ERR_INVITEONLYCHAN(Nick, ChanName) ":ircserv 473 " + Nick + " " + ChanName + " :Cannot join channel (+i)"

#define ERR_BANNEDFROMCHAN(Nick, ChanName) ":ircserv 474 " + Nick + " " + ChanName + " :Cannot join channel (+b)"

#define ERR_BADCHANNELKEY(Nick, ChanName) ":ircserv 475 " + Nick + " " + ChanName + " :Cannot join channel (+k)"

#define ERR_CHANOPRIVSNEEDED(Nick, ChanName) ":ircserv 482 " + Nick + " " + ChanName + " :You're not channel operator"

//#define ERR_UMODEUNKNOWNFLAG(Nick, Modechar) ":ircserv 501 " + Nick + " " + ModeChar + " :Unknown MODE flag"

//#define ERR_INVALIDKEY(Nick, ChanName) ":ircserv 525 " + Nick + " " + ChanName + " :Key is not well-formed"