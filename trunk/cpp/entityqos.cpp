#ifdef WITH_ENABLE_IF
namespace boost {

template <bool Condition, typename ResultType>
struct enable_if_c
{ };

template <typename ResultType>
struct enable_if_c <true, ResultType>
{
  typedef ResultType type;
};

template <typename Condition, typename ResultType>
struct enable_if : public enable_if_c <Condition::value, ResultType>
{};

} // boost
#endif // WITH_ENABLE_IF

namespace dds { namespace core {

/**
 * This class provides the basic behavior for Value types. 
 */
template <typename D>
class Value {
protected:
    Value() { }
    Value(const Value& p) : d_(p.d_) { }

public:
    template <typename ARG>
    Value(const ARG& arg) : d_(arg) { }

    template <typename ARG1, typename ARG2>
    Value(const ARG1& arg1, const ARG2& arg2) : d_(arg1, arg2) { }
    
    template <typename ARG1, typename ARG2, typename ARG3>
    Value(const ARG1& arg1, const ARG2& arg2, const ARG3& arg3)
    : d_(arg1, arg2, arg3) { }
    
public:
    ~Value() { }

public:
    Value& operator=(const Value& other) {
        if (this != &other) {
            d_ = other.d_;
        }
        return *this;
    }

    bool operator==(const Value& other) {
    	return (d_ == other.d_);
    }

    bool operator !=(const Value& other) {
    	return !(d_ == other.d_);
    }

public:
    const D* operator->() const { return d_; }
    D* operator->() { return &d_; }

    const D& delegate() const { return d_; }
    D& delegate() { return d_;}

    operator const D &() const { return d_; }
    operator D &() { return d_; }

protected:
    D d_;
};

} } /* dds / core */

namespace tdds { namespace core { namespace qos {


template<typename T, typename POLICY>
class HasPolicy
{
    template<typename U, void (U::*)(const POLICY &)> struct Check;
 
    template <typename U>
    static char func(Check<U, &U::policy> *);
 
    template <typename U>
    static int func(...);
 
  public:
    typedef HasPolicy type;
    enum { value = sizeof(func<T>(0)) == sizeof(char) };
};
/* Acts as a container for Qos policies allowing to set and retrieve all
 * the policies of an entity as a unit.
 */
template <typename DELEGATE>
class EntityQos : public dds::core::Value<DELEGATE> {
public:
    EntityQos() : dds::core::Value<DELEGATE>() { }

    EntityQos(const EntityQos& src) : dds::core::Value<DELEGATE>(src.delegate()) { }

// RTI_X : Why is this here?
    template <typename T>
    EntityQos(const EntityQos<T>& qos) :
        dds::core::Value<DELEGATE>(qos.delegate()) { }

public:
    ~EntityQos() { }

public:
    /**
     * Generic method for setting a policy applicable to this QoS object.
     * Available policies depends on the actual instantiation of the template
     * class, which might be DomainParticipantQos, TopicQos, PublisherQos, etc.
     *
     * @param p the policy to be set for this QoS instance.
     */
    template <typename POLICY>
    void policy(const POLICY& p) {
        this->dds::core::Value<DELEGATE>::delegate().policy(p);
    }

    /**
     * Generic method for obtaining the value set for a specific policy
     * belonging to this QoS instance.
     *
     * @return
     */
    template <typename POLICY>
#ifdef WITH_ENABLE_IF
    typename boost::enable_if<HasPolicy<DELEGATE, POLICY>,
                              const POLICY &>::type
#else
    const POLICY &
#endif
    policy() const {
        return this->delegate().template policy<POLICY>();
    }

    /**
     * Generic method for obtaining the value set for a specific policy
     * belonging to this QoS instance.
     *
     * @return
     */
    template <typename POLICY>
#ifdef WITH_ENABLE_IF
    typename boost::enable_if<HasPolicy<DELEGATE, POLICY>,
                              POLICY &>::type
#else
    POLICY &
#endif
    policy() {
        return this->delegate().template policy<POLICY>();
    }

    template <typename POLICY>
    EntityQos& operator << (const POLICY& p) {
        this->delegate().policy(p);
        return *this;
    }

    template <typename POLICY>
#ifdef WITH_ENABLE_IF
    typename boost::enable_if<HasPolicy<DELEGATE, POLICY>,
                              const EntityQos &>::type
#else
    const EntityQos &
#endif
    operator >> (POLICY& p) const {
        p = this->delegate().template policy<POLICY>();
        return *this;
    }

    template <typename T>
    EntityQos<DELEGATE>& operator = (const EntityQos<T>& other) {
        if (this != (EntityQos<DELEGATE>*)&other) {
            this->d_ = other.delegate();
        }
        return *this;
    }
};
} } } /* namespace tdds / namespace core / namespace qos */

namespace dds { namespace core { namespace policy {

struct Deadline      {};
struct LatencyBudget {};
struct Reliability   {};
struct Presentation  
{ 
  void set_pres(int i) { }; 
};

} } } // dds/core/policy

namespace idds { namespace qos {

class DataWriterQosImpl 
{
    dds::core::policy::Deadline deadline_;
    dds::core::policy::LatencyBudget latency_budget_;
    dds::core::policy::Reliability reliability_;
  public:
    template <typename POLICY> const POLICY& policy() const;
    template <typename POLICY> POLICY& policy();

    void policy(const dds::core::policy::Deadline      & d)  { deadline_ = d; }
    void policy(const dds::core::policy::LatencyBudget & lb) { latency_budget_ = lb; }
    void policy(const dds::core::policy::Reliability   & r)  { reliability_ = r; }
    void my_extension() {} 
};

template <>
const dds::core::policy::Deadline &
DataWriterQosImpl::policy<dds::core::policy::Deadline> () const {
  return deadline_;
}

template <>
const dds::core::policy::LatencyBudget &
DataWriterQosImpl::policy<dds::core::policy::LatencyBudget> () const {
  return latency_budget_;
}

template <>
const dds::core::policy::Reliability &
DataWriterQosImpl::policy<dds::core::policy::Reliability> () const {
  return reliability_;
}


template <>
dds::core::policy::Deadline &
DataWriterQosImpl::policy<dds::core::policy::Deadline> () {
  return deadline_;
}

template <>
dds::core::policy::LatencyBudget &
DataWriterQosImpl::policy<dds::core::policy::LatencyBudget> () {
  return latency_budget_;
}

template <>
dds::core::policy::Reliability &
DataWriterQosImpl::policy<dds::core::policy::Reliability> () {
  return reliability_;
}


} } // idds/qos

namespace dds { namespace pub { namespace qos {

  typedef tdds::core::qos::EntityQos<idds::qos::DataWriterQosImpl> 
    DataWriterQos;

} } } // dds/pub/qos

int main(void)
{
  dds::pub::qos::DataWriterQos dwqos;
  dds::core::policy::Deadline d;
  dds::core::policy::Presentation p; (void) p;
  //dwqos.policy(p); // Compile-time error.
  //dwqos << p;      // Compile-time error.
  
  //dwqos >> p;  // no compile-time error.
  p = dwqos.policy<dds::core::policy::Presentation>(); // no compile-time error, only link-time error.
  dwqos.policy<dds::core::policy::Presentation>().set_pres(10); // no compile-time error, only link-time error.
  
  dwqos >> d;
  d = dwqos.policy<dds::core::policy::Deadline>();
  dwqos->my_extension();
}

