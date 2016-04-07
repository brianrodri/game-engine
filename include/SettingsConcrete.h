#include <aetee/type_c.h>

class AccelComp;
class ComponentTuple;
class MimicComp;
class PositionComp;
class SimpleMotionComp;
class VelocityComp;

using ComponentList = aetee::type_sequence_t< AccelComp
                                            , ComponentTuple
                                            , MimicComp
                                            , PositionComp
                                            , SimpleMotionComp
                                            , VelocityComp
                                              >;

