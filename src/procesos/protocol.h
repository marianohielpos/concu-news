#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#define REQUEST 1
#define RESPONSE 2

#define TYPE_GET_CITY 1
#define TYPE_GET_CURRENCY 2
#define TYPE_SET_CITY 3
#define TYPE_SET_CURRENCY 4

#define TYPE_SUCESS 1
#define TYPE_ERROR 2

#define TEXT_SIZE 255


typedef struct message {
  long mtype;
  int type;
  char key[TEXT_SIZE];
  char value[TEXT_SIZE];
} message;

#endif /* __PROTOCOL_H__ */
