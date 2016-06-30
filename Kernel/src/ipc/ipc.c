#include <ipc.h>

//agrega un mensaje enviado por sender a la lista de mensajes de receiver
void new_message(uint64_t sender, uint64_t receiver, uint64_t size, void* message){
	
	msg_node newMsg = malloc(sizeof(struct message_t));
	newMsg->send_id = sender;
	newMsg->msg = message;
	newMsg->size = size;
	newMsg->next = NULL;
	
	addMessage(receiver, newMsg);
}

//devuelve el nodo con la lista de los mensajes de id receiver
mq_node get_mq(uint64_t receiver){
	mq_node current = mq->head;
	
	if(current == NULL){
		mq_node first = malloc(sizeof(mqNode));
		first->id = receiver;
		first->messages = NULL;
		first->last_message=NULL;
		first->next = NULL;
		mq->head = first;
		return first;
	}
	
	
	while(current->next != NULL ){
		if(current->id == receiver){
			return current;
		}
		current = current->next;
	}
	
	if(current->id == receiver){
		return current;
	}
	
	mq_node newNode = malloc(sizeof(mqNode));
	newNode ->id = receiver;
	newNode-> messages = NULL;
	newNode-> next = NULL;
	current->next = newNode;
	return newNode;
	
}

//agrega un mensaje
void addMessage(uint64_t receiver, msg_node message){
	mq_node node = get_mq(receiver);
	
	if(node->messages == NULL){
		node->messages = message;
		node->last_message = message;
		return;
	}
	
	node->last_message->next = message;
	node->last_message = message;

}

//imprime todos los mensajes de un id dado
// void print_messages(uint64_t receiver){
// 	mq_node node = get_mq(receiver);
// 	msg_node msg = node->messages;
// 	if(msg == NULL){
// 		printf("ES NULL\n");
// 	}
// 	printf("receiver %llu\n", receiver);
// 	while(msg != NULL){
// 		printf("sender: %llu\n", msg->send_id);
// 		msg = msg->next;
// 	}
// }

//devuelve el primer mensaje de la cola de mensajes de receiver enviado por sender
msg_node read_message(uint64_t receiver, uint64_t sender){
	mq_node node = get_mq(receiver);
	msg_node msgs= node ->messages;
	while(msgs != NULL){
		if(msgs->send_id == sender){
			return msgs;
		}
		msgs = msgs->next;
	}
	return NULL;
	
}

//devuelve todos los mensajes de receiver enviados por sender
msg_node read_messages(uint64_t receiver, uint64_t sender){
	mq_node node = get_mq(receiver);
	msg_node msgs = node -> messages;
	msg_node queue_first = NULL;
	msg_node queue = NULL;
	while(msgs != NULL){
		if(msgs->send_id == sender){
			if(queue == NULL){
				queue = malloc(sizeof(message_t));
				queue_first = queue;
				queue->send_id = msgs->send_id;
				queue->msg = msgs->msg;
				queue->size = msgs->size;
				queue->next = NULL;
			}else{
				msg_node newNode = malloc(sizeof(message_t));
				newNode->send_id = msgs->send_id;
				newNode->msg = msgs->msg;
				newNode->size = msgs->size;
				newNode->next = NULL;
				queue->next = newNode;
				queue = queue->next;
			}
		}
		msgs = msgs->next;
	}
	
	return queue_first;
}

//borra todos los mensajes de un receiver
void delete_mq(uint64_t receiver){
	mq_node prev = mq->head;
	mq_node current = mq->head->next;
	
	if(prev->id == receiver){
		mq->head = mq->head->next;
		return;
	}
	
	while(current!= NULL){
		if(current->id == receiver){
			prev->next = current->next;
			return;
		}
	}
}



