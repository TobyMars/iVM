

#include "list.h"

#include "debug.h"

#include "mem.h"

#include "token.h"

#include "obj.h"

#include "util.h"

#include "form.h"


#define LIST_SHOW_SUBLIST_ON true



lisp_list_s*
list_new(void)
{
    func_s();
    
    lisp_list_s *list = ml_malloc(sizeof(lisp_list_s));
    if (!list) out(fail, NULL);
    
    list->is_head = true;
    list->obj.type = OBJ_LIST;
    
    out(ok, list);
}



bool
list_add_token(lisp_list_s *list, token_s *token)
{
    func_s();
    
    
    if (!list || !token) {
        
        debug("NULL list or token \n");
        return false;
    }
    
    lisp_list_s *node = (lisp_list_s*)ml_malloc(sizeof(lisp_list_s));
    if (!node) return false;
    
    node->obj.type = OBJ_TYPE;
    
    memcpy(&node->obj.token, token, sizeof(token_s));
    
    if (!list->front) {
        
        list->next = node;
        list->front = node;
        node->front = list;
        node->next = list;
    }
    else {
        
        list->front->next = node;
        node->front = list->front;
        node->next = list;
        list->front = node;
    }
    
    func_ok();
    return true;
}


bool
list_add_object(lisp_list_s *list, object_s *obj)
{
    if (!list || !obj) return false;
    
    func_s();
    
    lisp_list_s *node = (lisp_list_s*)ml_malloc(sizeof(lisp_list_s));
    if (!node) return false;
    
    memcpy(&node->obj, obj, sizeof(object_s));
    
    
    if (!list->front) {
        
        debug("front is null \n");
        
        list->next = node;
        list->front = node;
        node->front = list;
        node->next = list;
    }
    else {
        
        list->front->next = node;
        node->front = list->front;
        node->next = list;
        list->front = node;
    }
    
    
    func_ok();
    return true;
}


bool
list_add_char_obj(lisp_list_s *list, char *ch)
{
    object_s obj;
    
    memset(&obj, 0, sizeof(object_s));
    
    obj.type = OBJ_CHARACTER;
    obj.character = ch;
    
    return list_add_object(list, &obj);
}


void
list_show(lisp_list_s *list)
{
    if (!list) return;
    
    func_s();
    
    debug("is_head: %d \n", list->is_head);
    
    if (list->is_nil) {
        
        debug("nil list \n");
        return;
    }
    
    
    lisp_list_s *l = list->next;
    
    while (l) {
        
        obj_show(&l->obj);
        
        if (l->obj.sub) {
            
            debug("  subform: \n");
            
#if LIST_SHOW_SUBLIST_ON
            form_s *f = l->obj.sub;
            //form_show(f);
            if (f && f->next) {
                
                //form_show(f->next);
                if (f->next->list) {
                    list_show(f->next->list->next);
                }
            }
#endif
            
        }
        
        
        l = l->next;
        
        if (l->is_head) break;
    }
    
    func_ok();
}


void
list_mark_type_specified(lisp_list_s *list)
{
    if (!list) return;
    
    func_s();
    
    debug("is_head: %d \n", list->is_head);
    
    if (list->is_nil) {
        
        debug("nil list \n");
        return;
    }
    
    
    lisp_list_s *l = list->next;
    
    while (l && l != list) {
        
        if (l->obj.type == OBJ_TYPE) {
            l->obj.is_specified_type = true;
        }
        
        l = l->next;
    }
    
    func_ok();
}


bool
list_copy(lisp_list_s *dst, lisp_list_s *src)
{
    
    func_s();
    
    if (!dst || !src) {
        
        goto FAIL;
    }
    
    lisp_list_s *l, *ll, *next, *front;
    
    l = src;
    ll = dst;
    while (l) {
        
        front = ll->front;
        memcpy(ll, l, sizeof(lisp_list_s));
        ll->front = front;
        
        l = l->next;
        if (l == src) break;
        
        next = (lisp_list_s*)ml_malloc(sizeof(lisp_list_s));
        if (!next) {
            
            goto FAIL;
        }
        
        ll->next = next;
        next->front = ll;
        
        ll = next;
    }
    
    next->next = dst;
    
    list_show(dst);
    
    func_ok();
    return true;
    
FAIL:
    func_fail();
    return false;
}


bool
list_is_head(lisp_list_s *node)
{
    if (!node) return false;
    
    return node->is_head;
}


bool
list_add_list(lisp_list_s *list, lisp_list_s *list_element)
{
    
    bool rt = false;
    
    func_s();
    
    
    if (!list || !list_element) goto FAIL;
    
    lisp_list_s *node = (lisp_list_s*)ml_malloc(sizeof(lisp_list_s));
    if (!node) return false;
    
    if (!list->front) {
        
        debug("front is null \n");
        
        list->next = node;
        list->front = node;
        node->front = list;
        node->next = list;
    }
    else {
        
        list->front->next = node;
        node->front = list->front;
        node->next = list;
        list->front = node;
    }
    
    node->obj.type = OBJ_TYPE;
    node->obj.subtype = OBJ_SUBTYPE_LIST_AS_ELEMENT;
    
    node->obj.list = ml_malloc(sizeof(lisp_list_s));
    if (!node->obj.list) goto FAIL;
    
    
    rt = list_copy(node->obj.list, list_element);
    
    
    out(ok, rt);
    
FAIL:
    out(fail, rt);
}




