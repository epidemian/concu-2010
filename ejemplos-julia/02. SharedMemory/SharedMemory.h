#ifndef SHAREDMEMORY_H_
#define SHAREDMEMORY_H_

#define SHM_OK			0
#define	ERROR_FTOK		-1
#define ERROR_SHMGET	-2
#define	ERROR_SHMAT		-3

#include	<sys/types.h>
#include	<sys/ipc.h>
#include	<sys/shm.h>


template <class T> class SharedMemory {

private:

	int 	shmId;
	T*		ptrDatos;

public:

	SharedMemory ();
	~SharedMemory ();
	int crear ( char *archivo,char letra );
	void liberar ();
	void escribir ( T dato );
	T leer ();

};

template <class T> SharedMemory<T> :: SharedMemory () {
}

template <class T> SharedMemory<T> :: ~SharedMemory () {
}

template <class T> int SharedMemory<T> :: crear ( char *archivo,char letra ) {

	// generacion de la clave
	key_t clave = ftok ( archivo,letra );
	if ( clave == -1 )
		return ERROR_FTOK;
	else {
		// creacion de la memoria compartida
		this->shmId = shmget ( clave,sizeof(T),0644|IPC_CREAT );

		if ( this->shmId == -1 )
			return ERROR_SHMGET;
		else {
			// attach del bloque de memoria al espacio de direcciones del proceso
			void* ptrTemporal = shmat ( this->shmId,NULL,0 );

			if ( ptrTemporal == (void *) -1 )
				return ERROR_SHMAT;
			else {
				this->ptrDatos = (T *) ptrTemporal;
				return SHM_OK;
			}
		}
	}
}


template <class T> void SharedMemory<T> :: liberar () {

	// detach del bloque de memoria
	shmdt ( (void *) this->ptrDatos );

	// consulta del estado de la memoria compartida
	shmid_ds estado;
	shmctl ( this->shmId,IPC_STAT,&estado );

	if ( estado.shm_nattch == 0 )
		// destruccion de la shared memory
		shmctl ( this->shmId,IPC_RMID,NULL );
}

template <class T> void SharedMemory<T> :: escribir ( T dato ) {
	* (this->ptrDatos) = dato;
}

template <class T> T SharedMemory<T> :: leer () {
	return ( *(this->ptrDatos) );
}


#endif /* SHAREDMEMORY_H_ */
