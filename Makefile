CC= gcc
CFLAGS= -Wall -ansi -pedantic -Iinclude
CPPFLAGS= 

OBJ= image.o  
ARCHIVES = libimage.a  

all  : $(OBJ)


image.o:	 src/image.c include/image.h\
		 include/objet.h include/exit_if.h\
		 include/classe.h include/point.h\
		 include/move_type.h include/type_obj.h 
		$(CC) -Iinclude $(CPPFLAGS) $(CFLAGS) -c src/image.c;\
		ar q libimage.a image.o



clean :
	rm -f $(OBJ) $(ARCHIVES)




