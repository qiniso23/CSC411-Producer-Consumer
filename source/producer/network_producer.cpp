#include "network_producer.h"

//***************************************************************************************************************************************************
network_producer::network_producer(network_buffer& _buffer)
	: producer(_buffer)
	, net_buffer_(_buffer)
{}

//***************************************************************************************************************************************************
void network_producer::run(int _num_students) 
{
	producer::run(_num_students);
	net_buffer_.set_finished();
}