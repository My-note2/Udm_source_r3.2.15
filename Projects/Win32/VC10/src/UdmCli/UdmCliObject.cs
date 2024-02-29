using System;
using System.Collections.Generic;
using System.Text;
using Udm.Native;
using Udm.Native.Uml;

namespace Udm.Native
{
    public class UdmCliObject : IUdmObject
    {
        public UdmObject backing;

        public UdmCliObject(UdmObject o)
        {
            backing = o;
        }
        public UdmCliObject(ObjectImpl o)
        {
            backing = new UdmObject(o);
        }
        public static void CheckCast(UdmObject a, Uml.Class meta)
        {
            if (!Udm.IsDerivedFrom(a.type(), meta))
            {
                throw new UdmException("Invalid cast from type '" + a.type().name().Get() + "' to type '" + meta.name().Get() + "'");
            }
        }
        public int id
        {
            get { return backing.uniqueId(); }
        }
        public bool Equals(UdmCliObject that)
        {
            return this.id == that.id;
        }
        public override bool Equals(object that)
        {
            if (that is UdmCliObject)
            {
                return Equals((UdmCliObject)that);
            }
            return false;
        }
        public override int GetHashCode()
        {
            return id;
        }
        public string type_name
        {
            get { return backing.type().name().Get(); }
        }
        public Uml.Class type
        {
            get { return backing.type(); }
        }
        public IUdmObject parent
        {
            get { 
            	UdmObject o = backing.GetParent();
                if (o.uniqueId() == 0)
                	return null;
                return new UdmCliObject(o);
            }
            
            set
            {
                if (value == null)
                    backing.__impl().setParent(Udm._null.__impl(), Udm.NULLPARENTROLE);
                else
                    backing.__impl().setParent((value as UdmCliObject).backing.__impl(), Udm.NULLPARENTROLE);
            }
        }
        public System.Collections.Generic.IEnumerable<IUdmObject> children
        {
            get
            {
                using (Object_set children = backing.GetChildObjects())
                {
                    return System.Linq.Enumerable.ToList(
                        System.Linq.Enumerable.Select<UdmObject, IUdmObject>
                        (children, o => new UdmCliObject(o)));
                }
            }

            set
            {
                using (ObjectImpl_vector children = new ObjectImpl_vector())
                {
                    foreach (IUdmObject child in value)
                    {
                        children.Add((child as UdmCliObject).backing.__impl());
                    }
                    backing.__impl().setChildren(Udm.NULLCHILDROLE, children);
                }
            }
        }
        public bool isInstance
        {
            get { return backing.isInstance(); }
        }
        public bool isSubtype
        {
            get { return backing.isSubtype(); }
        }
        public IUdmObject archetype
        {
            get
            {
                UdmObject o = backing.archetype();
                if (o.uniqueId() == 0) return null;
                return new UdmCliObject(o);
            }
        }
        public System.Collections.Generic.IEnumerable<IUdmObject> instances
        {
            get
            {
                using (Object_set instances = backing.instances())
                {
                    return System.Linq.Enumerable.ToList(
                        System.Linq.Enumerable.Select<UdmObject, IUdmObject>
                        (instances, o => new UdmCliObject(o)));
                }
            }
        }
        public System.Collections.Generic.IEnumerable<IUdmObject> derived
        {
            get
            {
                using (Object_set derived = backing.derived())
                {
                    return System.Linq.Enumerable.ToList(
                        System.Linq.Enumerable.Select<UdmObject, IUdmObject>
                        (derived, o => new UdmCliObject(o)));
                }
            }
        }
    }
}
namespace Udm
{
    public interface IUdmObject
    {
        int id { get; }
        // TODO: should be Class inherited from IUdmObject
        Udm.Native.Uml.Class type { get; }
        IUdmObject parent { get; set; }
        System.Collections.Generic.IEnumerable<IUdmObject> children { get; set; }

        bool isInstance { get; }
        bool isSubtype { get; }
        IUdmObject archetype { get; }
        System.Collections.Generic.IEnumerable<IUdmObject> instances { get; }
        System.Collections.Generic.IEnumerable<IUdmObject> derived { get; }
    }


    public interface ClassFactory<T> where T : IUdmObject
    {

        global::Udm.Native.Uml.Class meta { get; }

        global::System.Func<global::Udm.IUdmObject, T> ICast { get; }

        global::System.Func<global::Udm.IUdmObject, T> ICreate { get; }

        global::System.Func<global::Udm.IUdmObject, global::Udm.Native.Uml.CompositionChildRole, T> ICreateWithRole { get; }

    }
}
