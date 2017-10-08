/*  =========================================================================
    pkgfiler_remote - Remote Hostdir Manager

    ** WARNING *************************************************************
    THIS SOURCE FILE IS 100% GENERATED. If you edit this file, you will lose
    your changes at the next build cycle. This is great for temporary printf
    statements. DO NOT MAKE ANY CHANGES YOU WISH TO KEEP. The correct places
    for commits are:

     * The XML model used for this code generation: pkgfiler_remote.xml, or
     * The code generation script that built this file: ../exec/zproto_client_c
    ************************************************************************
    =========================================================================
*/

#ifndef PKGFILER_REMOTE_H_INCLUDED
#define PKGFILER_REMOTE_H_INCLUDED

#define ZPROTO_UNUSED(object) (void)object

#include <czmq.h>

#ifdef __cplusplus
extern "C" {
#endif

//  Opaque class structure
#ifndef PKGFILER_REMOTE_T_DEFINED
typedef struct _pkgfiler_remote_t pkgfiler_remote_t;
#define PKGFILER_REMOTE_T_DEFINED
#endif

//  @interface
//  Create a new pkgfiler_remote, return the reference if successful, or NULL
//  if construction failed due to lack of available memory.
pkgfiler_remote_t *
pkgfiler_remote_new (void);

//  Destroy the pkgfiler_remote and free all memory used by the object.
void
    pkgfiler_remote_destroy (pkgfiler_remote_t **self_p);

//  Return actor, when caller wants to work with multiple actors and/or
//  input sockets asynchronously.
zactor_t *
    pkgfiler_remote_actor (pkgfiler_remote_t *self);

//  Return message pipe for asynchronous message I/O. In the high-volume case,
//  we send methods and get replies to the actor, in a synchronous manner, and
//  we send/recv high volume message data to a second pipe, the msgpipe. In
//  the low-volume case we can do everything over the actor pipe, if traffic
//  is never ambiguous.
zsock_t *
    pkgfiler_remote_msgpipe (pkgfiler_remote_t *self);

//  Return true if client is currently connected, else false. Note that the
//  client will automatically re-connect if the server dies and restarts after
//  a successful first connection.
bool
    pkgfiler_remote_connected (pkgfiler_remote_t *self);

//  Used to connect to the server.                                                  
int 
    pkgfiler_remote_construct (pkgfiler_remote_t *self, const char *endpoint);

//  Used to connect to the server.                                                  
int 
    pkgfiler_remote_set_hostdir (pkgfiler_remote_t *self, const char *hostdir);

//  Enable verbose tracing (animation) of state machine activity.
void
    pkgfiler_remote_set_verbose (pkgfiler_remote_t *self, bool verbose);

//  Self test of this class
void
    pkgfiler_remote_test (bool verbose);
//  @end

#ifdef __cplusplus
}
#endif

#endif
