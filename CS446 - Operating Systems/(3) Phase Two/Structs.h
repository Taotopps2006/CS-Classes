#ifndef STRUCTS_H
#define STRUCTS_H

struct Process
{
	char component;
	string operation;
	int numberOfCycles;
};

struct pcb_thread
{
	string startLogMessage;
	string endLogMessage;
	int processTime;
};

#endif // STRUCTS_H