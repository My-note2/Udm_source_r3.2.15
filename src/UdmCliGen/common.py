
impl_namespace = "Udm.Impl."
interface_namespace = ""

attr_typemap = { 'String': 'string',
    'Boolean': 'bool',
    'Integer': 'long',
    'Real': 'double', }

def fq_iname(c):
    ret = "I" + c.name
    parent = c.parent
    while parent.id != 0:
        ret = parent.name + "." + ret
        parent = parent.parent
    return "global::" + interface_namespace + ret

def iter_parents(o, include_self=True):
    if include_self:
        yield o
    parent = o.parent
    while parent.id != 0:
        yield parent
        parent = parent.parent

def get_path(o, sep="."):
    ret = o.name
    parent = o.parent
    while parent.id != 0:
        ret = parent.name + sep + ret
        parent = parent.parent
    return ret

def fq_name(c):
    return "global::" + impl_namespace + get_path(c)

def get_descendants(o):
    from collections import deque
    ret = []
    q = deque()
    q.append(o)
    while q:
        o = q.pop()
        q.extend(o.children())
        ret.append(o)
    return ret

def role_metaname(role):
    return "meta_" + role_name(role)

def role_name(role):
    if role.name != "":
        return role.name
    orole = role.parent.children()[0] if role == role.parent.children()[1] else role.parent.children()[1]
    # TODO: can AssociationRoles have blank names?
    suffix_map = { "CompositionChildRole": "_parent", "CompositionParentRole": "_children", "AssociationRole": "" }
    if orole.type.name == "CompositionParentRole" and role.max == 1:
        suffix_map = { "CompositionParentRole": "_child" }
    ret = ""
    if orole.name:
        ret = orole.name + "_" 
    return ret + role.target.name + suffix_map[orole.type.name]
