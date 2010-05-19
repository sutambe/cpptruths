/** \file linkdef.c
    \brief Implements LinkDef data structure.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "linkdef.h"

void linkdef_initialise(LinkDef *ld, int size)
{
    assert(ld && (size > 0));
    hash_initialise(&ld->links_hash,size,listelement_tostring);
    hash_initialise(&ld->defs_hash,size,listelement_tostring);
    hash_initialise(&ld->symbols_hash,size,listelement_tostring);
}
void linkdef_deinitialise(LinkDef *ld)
{
    assert(ld);
    hash_deinitialise(&ld->links_hash,destroy);
    hash_deinitialise(&ld->defs_hash,destroy);
    hash_deinitialise(&ld->symbols_hash,destroy);
}

ListElement *linkdef_assign(ListElement *any,
                             const char *funcname,
                             const char *libname,
                             const StringMemoryType requested_strmem)
{
    assert(any && funcname && libname);

    switch(any->element_type)
    {
        case LINK:
        {
            if(MEM_OWNER==any->strmem)
            {
                free((char *)any->element.link.funcname);
                free((char *)any->element.link.funcname);
            }
            if(MEM_OWNER==requested_strmem)
            {
                any->element.link.funcname=strdup(funcname);
                any->element.link.libname=strdup(libname);
            }
            else
            {
                any->element.link.funcname=funcname;
                any->element.link.libname=libname;
            }
            break;
        }
        case DEFINITION:
        {
            if(MEM_OWNER==any->strmem)
            {
                free((char *)any->element.def.funcname);
                free((char *)any->element.def.funcname);
            }
            if(MEM_OWNER==requested_strmem)
            {
                any->element.def.funcname=strdup(funcname);
                any->element.def.libname=strdup(libname);
            }
            else
            {
                any->element.def.funcname=funcname;
                any->element.def.libname=libname;
            }
            break;
        }

        case SYMBOL:
            break;
    }
    return any;
}

ListElement *linkdef_find_link(LinkDef *ld,
                                const char *link_funcname,
                                const char *link_libname)
{
    ListElement element, *link;
    assert(ld && link_funcname && link_libname);
    
    list_element_initialise(&element);
    element.element_type=LINK;
    element.element.link.funcname=link_funcname;
    element.element.link.libname=pathsubstr(link_libname);

    /* For maintainers: This needs to be called twice with different 
       libname because calculation of hash value is dependent on
       library path. The desired record could be situated in any of the
       two buckets. If you change anything over here, you need to make changes
       where hash_search, hash_erase is used. */
    // DEBUG fprintf(stderr,"\n\n linkdef_find_link: before hash_search\n\n");
    link=hash_search(&ld->links_hash,&element,compare_with_tablename);
    if(!link)
    {
        element.element.link.libname=link_libname;
        link=hash_search(&ld->links_hash,&element,compare_with_tablename);
    }
    return link;
}

ListElement *linkdef_find_def(LinkDef *ld,
                                const char *def_funcname,
                                const char *def_libname)
{
    ListElement element, *def;
    assert(ld && def_funcname && def_libname);

    list_element_initialise(&element);
    element.element_type=DEFINITION;
    element.element.def.funcname=def_funcname;
    element.element.def.libname=pathsubstr(def_libname);

    /* For maintainers: This needs to be called twice with different 
       libname because calculation of hash value is dependent on
       library path. The desired record could be situated in any of the
       two buckets. If you change anything over here, you need to make changes
       where hash_search, hash_erase is used. */
    def=hash_search(&ld->defs_hash,&element,compare_without_tablename);
    if(!def)
    {
        element.element.def.libname=def_libname;
        def=hash_search(&ld->defs_hash,&element,compare_without_tablename);
    }
    return def;
}

ListElement *linkdef_add_link(LinkDef *ld,
                                const char *link_funcname,
                                const char *link_libname,
                                const StringMemoryType requested_strmem)
{
    ListElement element, *link;

    assert(ld && link_funcname && link_libname);

    list_element_initialise(&element);
    element.element_type=LINK;
    element.element.link.funcname=link_funcname;
    element.element.link.libname=link_libname;

    link=hash_add(&ld->links_hash,&element,(MEM_OWNER==requested_strmem)? deepcopy : shallowcopy);
    assert(link);
    link->strmem=requested_strmem;
        
    return link;
}

ListElement *linkdef_add_def(LinkDef *ld,
                                const char *def_funcname,
                                const char *def_libname,
                                const StringMemoryType requested_strmem)
{
    ListElement element, *def;

    assert(ld && def_funcname && def_libname);

    list_element_initialise(&element);
    element.element_type=DEFINITION;
    element.element.def.funcname=def_funcname;
    element.element.def.libname=def_libname;

    def=hash_add(&ld->defs_hash,&element,(MEM_OWNER==requested_strmem)? deepcopy : shallowcopy);
    assert(def);
    def->strmem=requested_strmem;
        
    return def;
}

/* The link's parent can be either DEFINITION type or LINK type. */
ListElement *linkdef_search_parentof_linktodef(ListElement *link, ListElement *def)
{
    ListElement *parent;
    assert(link && def);

    if(link == def->element.def.backlink) return def;
    else parent=def->element.def.backlink;

    while(parent && (link != parent->element.link.backlink))
          parent=parent->element.link.backlink;

    return parent;
}

ListElement * linkdef_drop_linkfromdef(ListElement *link, ListElement *def)
{
    ListElement *linkparent;
    assert(link && def);

    linkparent=linkdef_search_parentof_linktodef(link,def);
    if(!linkparent) return 0;
    else
    {
        switch(linkparent->element_type)
        {
            case LINK:
                linkparent->element.link.backlink=link->element.link.backlink;
                link->element.link.currentdef=0;
                link->element.link.backlink=0;
                break;

            case DEFINITION:
                linkparent->element.def.backlink=link->element.link.backlink;
                link->element.link.currentdef=0;
                link->element.link.backlink=0;
                break;
            case SYMBOL:
                /*  Unused right now. */
                break;
        }
        return link;
    }
}

int linkdef_erase_link(LinkDef *ld,const char *link_funcname,
                       const char *link_libname)
{
    ListElement element;
    ListElement *link, *def;
    int retval;

    link=linkdef_find_link(ld,link_funcname,link_libname);
    if(!link) return 1;

    def=link->element.link.currentdef;
    if(def)
    {
        link=linkdef_drop_linkfromdef(link,def);
        if(!link) return 1;
    }

    list_element_initialise(&element);
    element.element_type=LINK;
    element.element.link.funcname=link_funcname;
    element.element.link.libname=pathsubstr(link_libname);
    
    retval=hash_erase(&ld->links_hash,&element,compare_with_tablename,destroy);
    if(retval)
    {
        element.element.link.libname=link_libname;
        retval=hash_erase(&ld->links_hash,&element,compare_with_tablename,destroy);
    }
    return retval;
}

int linkdef_erase_def(LinkDef *ld,const char *def_funcname,
                      const char *def_libname)
{
    ListElement element;
    ListElement *def;
    int retval;
    
    assert(ld && def_funcname && def_libname);

    def=linkdef_find_def(ld,def_funcname,def_libname);
    if(!def) return 1;
    if(def->element.def.backlink) return 1;

    list_element_initialise(&element);
    element.element_type=DEFINITION;
    element.element.def.funcname=def_funcname;
    element.element.def.libname=pathsubstr(def_libname);
    
    /* For maintainers: This needs to be called twice with different 
       libname because calculation of hash value is dependent on
       library path. The desired record could be situated in any of the
       two buckets. If you change anything over here, you need to make changes
       where hash_search, hash_erase is used. */
    retval=hash_erase(&ld->defs_hash,&element,compare_without_tablename,destroy);
    if(retval)
    {
        element.element.def.libname=def_libname;
        retval=hash_erase(&ld->defs_hash,&element,compare_without_tablename,destroy);
    }
    return retval;
}

ListElement *linkdef_insert_linktodef(ListElement *link, ListElement *def)
{
    assert(link && def);

    link->element.link.currentdef=def;
    link->element.link.backlink=def->element.def.backlink;
    def->element.def.backlink=link;

    return link;
}

ListElement *linkdef_add_table(LinkDef *ld,
                                const char *tablename,
                                const char *table_funcname,
                                const char *table_libname,
                                const StringMemoryType requested_strmem)
{
    ListElement element, *def;

    assert(ld && tablename && table_funcname && table_libname);

    list_element_initialise(&element);
    element.element_type=DEFINITION;
    element.element.def.funcname=table_funcname;
    element.element.def.libname=table_libname;
    element.element.def.table_name=tablename;

    def=hash_add(&ld->defs_hash,&element,(MEM_OWNER==requested_strmem)? deepcopy : shallowcopy);
    assert(def);
    def->strmem=requested_strmem;
    
    return def;
}

ListElement *linkdef_find_table(LinkDef *ld,
                                const char *tablename,
                                const char *table_funcname,
                                const char *table_libname)
{
    ListElement element, *def;

    assert(ld && tablename && table_funcname && table_libname);

    list_element_initialise(&element);
    element.element_type=DEFINITION;
    element.element.def.funcname=table_funcname;
    element.element.def.libname=pathsubstr(table_libname);
    element.element.def.table_name=tablename;

    /* For maintainers: This needs to be called twice with different 
       libname because calculation of hash value is dependent on
       library path. The desired record could be situated in any of the
       two buckets. If you change anything over here, you need to make changes
       where hash_search, hash_erase is used. */
    def=hash_search(&ld->defs_hash,&element,compare_with_tablename);
    if(!def)
    {
        element.element.def.libname=table_libname;
        def=hash_search(&ld->defs_hash,&element,compare_with_tablename);
    }
    return def;
}

int compare_without_tablename(const ListElement *to_search,const ListElement *inlist)
{
    int samelname, samefname;

    assert(to_search && inlist);

    if((LINK==to_search->element_type) && (LINK==inlist->element_type))
    {
        assert(to_search->element.link.funcname && inlist->element.link.funcname);
        assert(to_search->element.link.libname && inlist->element.link.libname);

        samefname=!strcmp(to_search->element.link.funcname,inlist->element.link.funcname);
        samelname=samefname && !pathendcmp(to_search->element.link.libname,inlist->element.link.libname);

        return samefname && samelname;
    }
    else if((DEFINITION==to_search->element_type) && (DEFINITION==inlist->element_type))
    {
        assert(to_search->element.def.funcname && inlist->element.def.funcname);
        assert(to_search->element.def.libname && inlist->element.def.libname);

        samefname=!strcmp(to_search->element.def.funcname,inlist->element.def.funcname);
        samelname=samefname && !pathendcmp(to_search->element.def.libname,inlist->element.def.libname);

        return samefname && samelname;
    }
    else if((SYMBOL==to_search->element_type) && (SYMBOL==inlist->element_type))
    {
        assert(to_search->element.symbol.sym_name && inlist->element.symbol.sym_name);

        return !strcmp(to_search->element.symbol.sym_name,
                       inlist->element.symbol.sym_name);
    }
    else
    {
        return 0;
    }
}


int compare_with_tablename(const ListElement *to_search,const ListElement *inlist)
{
    int samelname, samefname, sametname;

    assert(to_search && inlist);

    if((LINK==to_search->element_type) && (LINK==inlist->element_type))
    {
        assert(to_search->element.link.funcname && inlist->element.link.funcname);
        assert(to_search->element.link.libname && inlist->element.link.libname);

        samefname=!strcmp(to_search->element.link.funcname,inlist->element.link.funcname);
        samelname=samefname && !pathendcmp(to_search->element.link.libname,inlist->element.link.libname);

        return samefname && samelname;
    }
    else if((DEFINITION==to_search->element_type) && (DEFINITION==inlist->element_type))
    {
        assert(to_search->element.def.funcname && inlist->element.def.funcname);
        assert(to_search->element.def.libname && inlist->element.def.libname);

        samefname=!strcmp(to_search->element.def.funcname,inlist->element.def.funcname);
        samelname=samefname && !pathendcmp(to_search->element.def.libname,inlist->element.def.libname);

        if(0==inlist->element.def.table_name && 0==to_search->element.def.table_name)
        {
            return samefname && samelname;
        }
        else if(inlist->element.def.table_name && to_search->element.def.table_name)
        {
            sametname=samelname && !strcmp(to_search->element.def.table_name,inlist->element.def.table_name);
            return samefname && samelname && sametname;
        }
        else
        {
            return 0;
        }

/*      This code was active when we used to make a entry in def without libname
        during redirect_lookup. The special care was necessary due to NULL libname
        for some defs and for some it is not. Now there is no such inconsistency.

        assert(to_search->element.def.funcname && inlist->element.def.funcname);
        samefname=!strcmp(to_search->element.def.funcname,inlist->element.def.funcname);

        if(0==inlist->element.def.libname && 0==to_search->element.def.libname)
        {
            assert(to_search->element.def.libname && inlist->element.def.libname);
            return samefname;
        }
        else if(inlist->element.def.libname && to_search->element.def.libname)
        {
            samelname=!strcmp(to_search->element.def.libname,inlist->element.def.libname);
            return samefname && samelname;
        }
        else
        {
            assert(to_search->element.def.libname);
            assert(inlist->element.def.libname);
            return 0;
        }
*/
    }
    else if((SYMBOL==to_search->element_type) && (SYMBOL==inlist->element_type))
    {
        assert(to_search->element.symbol.sym_name && inlist->element.symbol.sym_name);

        return !strcmp(to_search->element.symbol.sym_name,
                       inlist->element.symbol.sym_name);
    }
    else
    {
        return 0;
    }
}

void shallowcopy(ListElement *dest,const ListElement *source)
{
    assert(dest && source);
    *dest=*source;
    dest->strmem=MEM_REFERENCE;
}

void deepcopy(ListElement *dest,const ListElement *source)
{
    assert(dest && source);
    *dest=*source;
    dest->strmem=MEM_OWNER;
    switch(source->element_type)
    {
        case LINK:
        {
            dest->element.link.funcname=(const char *)strdup(source->element.link.funcname);
            dest->element.link.libname=(const char *)strdup(source->element.link.libname);
            break;
        }
        case DEFINITION:
        {
            dest->element.def.funcname=(const char *)strdup(source->element.def.funcname);
            dest->element.def.libname=(const char *)strdup(source->element.def.libname);
            if(source->element.def.table_name)
                dest->element.def.table_name=(const char *)strdup(source->element.def.table_name);

            break;
        }
        case SYMBOL:
        {
            dest->element.symbol.sym_name=(const char *)strdup(source->element.symbol.sym_name);
            dest->element.symbol.libname=(const char *)strdup(source->element.symbol.libname);
            dest->element.symbol.caller_libname=(const char *)strdup(source->element.symbol.caller_libname);
            break;
        }
        default:
        {
            exit(EXIT_FAILURE);
        }
    }
}

void destroy(ListElement *l)
{
    assert(l);
    if(MEM_OWNER != l->strmem)
        return;

    switch(l->element_type)
    {
        case LINK:
        {
                free((char *)l->element.link.funcname);
                free((char *)l->element.link.libname);
                break;
        }
        case DEFINITION:
        {
            free((char *)l->element.def.funcname);
            free((char *)l->element.def.libname);
            if(l->element.def.table_name)
               free((char *)l->element.def.table_name);

            break;
        }
        case SYMBOL:
        {
            free((char *)l->element.symbol.sym_name);
            free((char *)l->element.symbol.libname);
            free((char *)l->element.symbol.caller_libname);
            break;
        }
        default:
        {
            exit(EXIT_FAILURE);
        }
    }
}

const char *listelement_tostring(const ListElement *elm)
{
    static char string[1000];

    assert(elm);
    switch(elm->element_type)
    {
        case LINK:
        {
            sprintf(string,"%s%s",elm->element.link.funcname,elm->element.link.libname);
            return (string);
        }
        case DEFINITION:
        {
            sprintf(string,"%s%s",elm->element.def.funcname,elm->element.def.libname);
            return (string);
        }
        case SYMBOL:
        {
            sprintf(string,"%s",elm->element.symbol.sym_name);
            return (string);
        }
        default:
            return 0;
    }
}

int pathendcmp(const char *s1, const char *s2)
{
    const char *s1_ptr;
    const char *s2_ptr;
    int retval=0;

    //if(0==*s1 || 0==*s2) return -999;
    s1_ptr = s1 + strlen(s1)-1;
    s2_ptr = s2 + strlen(s2)-1;

    while(s1 <= s1_ptr && s2 <= s2_ptr && (*s1_ptr == *s2_ptr))
    { --s1_ptr; --s2_ptr; };

    if(s2 > s2_ptr)
    {
        if(s1 > s1_ptr) retval=0;
        else if('/'==*s1_ptr) retval=0;
        else retval=-999;
    }
    else if(s1 > s1_ptr)
    {
        if(s2 > s2_ptr) retval=0;
        else if('/'==*s2_ptr) retval=0;
        else retval=-999;
    }
    else retval=-999;

    return retval;
}

const char *pathsubstr(const char *str)
{
    const char *str_ptr;
    
    str_ptr=str + strlen(str)-1;
    while(str_ptr >= str && '/' != *str_ptr) --str_ptr;
    
    return ++str_ptr;
}


void linkdef_show(const ListElement *element)
{
    assert(element);
    switch(element->element_type)
    {
        case LINK:
        {
            printf("LINK ");
            printf("FUNCNAME: %s\n",element->element.link.funcname);
            printf("LIBNAME: %s\n",element->element.link.libname);
            printf("GOT_addr: %p\n",element->element.link.GOT_addr);
            printf("OFFSET: %d\n",element->element.link.offset);
            break;
        }
        case DEFINITION:
        {
            printf("DEFINITION ");
            if(element->element.def.table_name)
               printf("TABLENAME: %s\n",element->element.def.table_name);
            printf("FUNCNAME: %s\n",element->element.def.funcname);
            printf("LIBNAME: %s\n",element->element.def.libname);
            printf("ADDRESS: %p\n",element->element.def.address);
            break;
        }
        case SYMBOL:
        {
            printf("SYMBOL\n");
            printf("Symbol name: %s\n",element->element.symbol.sym_name);
            printf("Caller libname: %s\n",element->element.symbol.caller_libname);
            printf("Libname: %s\n",element->element.symbol.libname);
            printf("Symbol address: %p\n",element->element.symbol.sym_addr);
            break;
        }
    }
}
