## ROS Client Library for MicroController (RCLUC) and ROS MiddleWare for Micros (RMWU)
This is a ROS2 client for micro controllers. It strives to provide the same programming paradigms offered by the ROS RCL interface while also taking into account the limitations on resource constrained devices.
It is meant to be a generic interface that can have the DDS/DDS-XRCE implementation swapped out underneath.  

These should be the design principles that we keep in mind as we develop this library:  
- No dynamic allocation should happen within this library. We should also be conscious of stack usage as well.
- Similar to the ROS2.0 RCL and RMW interfaces, we will provide one interface for our users to build off of and another interface for people looking to implement their own transport layer via DDS-XRCE or other protocols
- The user should not have to change any application code if the underlying RMWU implementation is changed out. There may be some implementation specific configuration that will have to get fronted to the user but we should decouple that as much as possible from the RCLUC APIs.
- The RMWU interface should be agnostic of the implementation details for the underlying transport layer. While we're targeting DDS-XRCE using micro-RTPS for our first implementation, nothing should prevent someone from implementing their own custom protocol and agent to connect to a ROS graph.
- We should reduce the amount or redundant storage of information between RCLUC and RMWU. For example, things like a node's name shouldn't have to be stored in both layers if it can be exposed so that one layer can get it from the other.
- We should restrict the ability of the RMWU layer to change data from the RCLUC layer. It's okay if it needs to access it or get it passed in, but it should not have the ability to change data owned by that layer unless given explicit permission.
- Once launched, the API for the RCLUC must be very rigid. Any breaking change in this interface makes it difficult for our users to upgrade to the latest without the need for manual migration work. Similarly, the RMWUC interface should remain as rigid as possible so that any existing RMWU implementation will continue to work with new versions of the library without the need for migration.  
