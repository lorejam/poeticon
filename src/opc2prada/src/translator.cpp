// -*- mode:C++; tab-width:4; c-basic-offset:4; indent-tabs-mode:nil -*-

/*
 * Copyright: (C) 2012-2015 POETICON++, European Commission FP7 project ICT-288382
 * Copyright: (C) 2014 VisLab, Institute for Systems and Robotics,
 *                Istituto Superior Técnico, Universidade de Lisboa, Lisbon, Portugal
 * Author: Pedro Vicente <pvicente@isr.ist.utl.pt>
 * CopyPolicy: Released under the terms of the GNU GPL v2.0
 *
 */

#include "translator.h"

using namespace yarp::os;
using namespace std;

// Translator Module
double TranslatorModule::getPeriod() {
    return 0;
}

bool TranslatorModule::interruptModule() {

    cout << "Interrupting your module, for port cleanup" << endl;

    rpc_port.interrupt();
    port_broad.interrupt();
	translatorPort.interrupt();

    return true;
}

bool   TranslatorModule::close() {

    /* optional, close port explicitly */
    cout << "Calling close function\n";

    readingThread->stop();
    delete readingThread;
    rpc_port.close();
    port_broad.close();
	translatorPort.close();

    return true;
}
bool   TranslatorModule::updateModule() {
	Bottle receive,dataBase,ids2,*idsp;
	receive.clear();
	receive = * (translatorPort.read());  //block

	cout << "Write to file the world state" << endl;
	readingThread->guard.lock();
	dataBase = readingThread->_data;
	ids2 = readingThread->_ids;
	readingThread->guard.unlock();

	cout << "after copy Data Bottle" << endl;
	if(dataBase.size()>0 && (dataBase.get(1).asString()!="empty")) {
		ofstream myfile,myfile2;
		myfile.open ("symbols.dat");
		myfile2.open ("state.dat");
		idsp = ids2.get(1).asList();
		idsp = idsp->get(1).asList();
		cout << "ids: " << idsp->toString().c_str() << endl;
		for(int i=1;i<dataBase.size();i++){ // for each object
			cout <<"Object ID: " << idsp->get((i-1)).asInt() << endl;
			Bottle *objecto = dataBase.get(i).asList();
			for(int j=0;j<objecto->size();j++) { // for each properties
				Bottle *propriedade = objecto->get(j).asList();
				cout << "switch"<< propriedade->get(0).asString().c_str() << endl;
				switch(propriedade->get(0).asVocab())	{
					case CMD_NAME:{
						//cout << "name:" << propriedade->get(1).asString().c_str() << endl;
						break;
					}
					case CMD_DESC: {
						//cout << "desc:" << endl;//propriedade->get(1).asString().c_str() << endl;
						break;
					}
					case CMD_POS: {
						//cout << "pos:" << endl;//propriedade->get(1).asString().c_str() << endl;
						break;
					}
					case CMD_ON_T: {
						cout << "on_t:" << endl;//propriedade->get(1).asString().c_str() << endl;
						Bottle *ontop = propriedade->get(1).asList();
						for(int k=0; k < ontop->size(); k++){
							myfile << idsp->get((i-1)).asInt() <<"_on_" <<ontop->get(k).asInt() << " 0 primitive binary " << endl;
							myfile2 << idsp->get((i-1)).asInt() <<"_on_" <<ontop->get(k).asInt() <<"() ";
						}
						break;
					}
					case CMD_REAW: {
						cout << "reaw:" << endl;//propriedade->get(1).asString().c_str() << endl;
						Bottle *reachable = propriedade->get(1).asList();
						for(int k=0; k < reachable->size(); k++){
							myfile << idsp->get((i-1)).asInt() <<"_isreachable_with_" <<reachable->get(k).asInt() << " 0 primitive binary " << endl;
							myfile2 << idsp->get((i-1)).asInt() <<"_isreachable_with_" <<reachable->get(k).asInt() <<"() ";
						}
						break;
					}
					case CMD_PULW: {
						cout << "pulw:" << endl;//propriedade->get(1).asString().c_str() << endl;
						Bottle *pullable = propriedade->get(1).asList();
						for(int k=0; k < pullable->size(); k++){
							myfile  << idsp->get((i-1)).asInt() <<"_ispullable_with_" <<pullable->get(k).asInt() << " 0 primitive binary " << endl;
							myfile2 << idsp->get((i-1)).asInt() <<"_ispullable_with_" <<pullable->get(k).asInt() <<"() ";
						}
						break;
					}
					case CMD_TOUC: {
						cout << "touc:" << endl;//propriedade->get(1).asString().c_str() << endl;
						Bottle *touch = propriedade->get(1).asList();
						for(int k=0; k < touch->size(); k++){
							myfile  << idsp->get((i-1)).asInt() <<"_touch_" <<touch->get(k).asInt() << " 0 primitive binary " << endl;
							myfile2 << idsp->get((i-1)).asInt() <<"_touch_" <<touch->get(k).asInt() <<"() ";
						}
						break;
					}
					case CMD_ISH: {
						cout << "ish:" << endl;//propriedade->get(1).asString().c_str() << endl;
						if(propriedade->get(1).asString() == "true") {
							myfile << idsp->get((i-1)).asInt() <<"_ishand" << " 0 primitive binary " << endl;
							myfile2 << idsp->get((i-1)).asInt() <<"_ishand" <<"() ";
						}
						break;
					}
					case CMD_FREE: {
						cout << "Free:" << endl;//propriedade->get(1).asString().c_str() << endl;
						if(propriedade->get(1).asString() == "true") {
							myfile << idsp->get((i-1)).asInt() <<"_clearhand" << " 0 primitive binary " <<endl;
							myfile2 << idsp->get((i-1)).asInt() <<"_clearhand" <<"() ";
						}
						break;
					}
					case CMD_IN_H: {
						cout << "in_h:" << endl;//propriedade->get(1).asString().c_str() << endl;
						if(propriedade->get(1).asString() == "right") {
							myfile << idsp->get((i-1)).asInt() <<"_inhand_" << " 0 primitive binary " <<endl;
							myfile2 << idsp->get((i-1)).asInt() <<"_inhand_" <<"() ";

							myfile << idsp->get((i-1)).asInt() <<"_istool" << " 0 primitive binary " <<endl;
							myfile2 << idsp->get((i-1)).asInt() <<"_istool" <<"() ";
						}
						if(propriedade->get(1).asString() == "left") {
							myfile << idsp->get((i-1)).asInt() <<"_inhand_" << " 0 primitive binary " <<endl;
							myfile2 << idsp->get((i-1)).asInt() <<"_inhand_" <<"() ";
							myfile << idsp->get((i-1)).asInt() <<"_istool" << " 0 primitive binary " <<endl;
							myfile2 << idsp->get((i-1)).asInt() <<"_istool" <<"() ";
						}
						break;
					}

					default: {
						cout << "not known attribute..." << endl;
					}
				}
			}
		}
		myfile.close();
		myfile2.close();
		
	}
	Bottle &send = translatorPort.prepare();
	send.clear();
	send.addString("Done");
	cout << "DONE" << endl;
	translatorPort.write();
    return true;
}

bool   TranslatorModule::configure(yarp::os::ResourceFinder &rf) {

    /* module name */
    moduleName = rf.check("name", Value("translator"),
                          "Module name (string)").asString();

    setName(moduleName.c_str());

    /* port names */
	translatorPortName  = "/" + moduleName + "/port:io";
	
    /* open ports */
    if (!translatorPort.open(
            translatorPortName.c_str()))
    {
        cout << getName() << ": unable to open port"
        << translatorPortName << endl;
        return false;
    }

	/* OPC comunication*/

	string broadname = "/" + moduleName + "/broadcast:i";
	/* open broad port */
    if (!port_broad.open(
            broadname.c_str()))
    {
        cout << getName() << ": unable to open port"
        << broadname << endl;
        return false;
    }
	string RpcClientname = "/" + moduleName + "/rpcClient";
	/* open rpc client port */
    if (!rpc_port.open(
            RpcClientname.c_str()))
    {
        cout << getName() << ": unable to open port"
        << RpcClientname << endl;
        return false;
    }

    /* Rate thread period */
    threadPeriod = rf.check("threadPeriod", Value(0.033),
        "Control rate thread period key value(double) in seconds ").asDouble();

    /* Create the reading rate thread */
    readingThread = new Thread_read(&port_broad,
                                &rpc_port,
                                threadPeriod);

    /* Starts the thread */
    if (!readingThread->start()) {
        delete readingThread;
        return false;
    }

    return true;
}


//// Thread_read

//constructor thread_read

Thread_read::Thread_read(BufferedPort<Bottle> * broad_port,RpcClient * rpc,int r)
: RateThread(r) {
	_port_broad = broad_port;
	_rpc_port = rpc;
	firstTime = true;
};

bool Thread_read::threadInit(){
	bool test=true;
	printf("Starting reading thread...Waiting for connection...\n");
		
	while(test){
		test = _port_broad->getInputCount()==0 || _rpc_port->getOutputCount()==0 ;
		Time::delay(0.1);
	}
	printf("Connection Done!");

	Bottle *received,rpc_cmd,rpc_response,aux;
    Bottle cmd,response;

	// Start syncronous broadcast to initialize _data and _ids
	cmd.clear();
	cmd.addVocab(Vocab::encode("sync"));
	cmd.addVocab(Vocab::encode("start"));
	cmd.add(0.1);
	_rpc_port->write(cmd,response);
	// read database and ask ids
	rpc_cmd.clear();
	rpc_cmd.addVocab(Vocab::encode("ask"));
	aux.addString("all");
	rpc_cmd.addList() = aux;
	received = _port_broad->read();
	// Stop syncronous broadcast
	cmd.clear();
	cmd.addVocab(Vocab::encode("sync"));
	cmd.addVocab(Vocab::encode("stop"));
	_rpc_port->write(cmd,response);
	Time::delay(1);
	// Start asyncronous
	_rpc_port->write(rpc_cmd,rpc_response);
	cmd.clear();
	cmd.addVocab(Vocab::encode("async"));
	cmd.addVocab(Vocab::encode("on"));
	_rpc_port->write(cmd,response);
	Time::delay(1);

	guard.lock();
	_data = *(received);
	_ids = rpc_response;
	guard.unlock();
	cout << "Initial Database - Check" << endl;
    return true;
}

void Thread_read::afterStart(bool s){
    if (s)
        printf("Thread1 started successfully\n");
    else
        printf("Thread1 did not start\n");
}

void Thread_read::run(){
    Bottle *received,rpc_cmd,rpc_response,aux;
    cout << "Run thread1" << endl;
    rpc_cmd.addVocab(Vocab::encode("ask"));
    aux.addString("all");
    rpc_cmd.addList() = aux;
    received = _port_broad->read();
    _rpc_port->write(rpc_cmd,rpc_response);
    guard.lock();
    _data = *(received);
    _ids = rpc_response;
    guard.unlock();
    cout << "Run thread1 - after read" << endl;
}
