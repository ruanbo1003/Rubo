/*
 * LogicNode.hpp
 *
 *      Author: ruanbo
 */

#ifndef SERVER_GATE_LOGICNODE_HPP_
#define SERVER_GATE_LOGICNODE_HPP_

#include "comm/comm.hpp"
#include "util/User.hpp"

class LogicNode
{
public:
    UserPtr _client;

public:
    LogicNode();
    virtual ~LogicNode();
};

using LogicNodePtr = std::tr1::shared_ptr<LogicNode>;

#endif /* SERVER_GATE_LOGICNODE_HPP_ */
