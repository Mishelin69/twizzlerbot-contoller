idf_component_register(
    SRCS
        src/nanopb/pb_common.c
        src/nanopb/pb_decode.c
        src/nanopb/pb_encode.c

        src/protodefs/PeriodicStatusUpdate.pb.c
        src/protodefs/Request.pb.c
        src/protodefs/Reservation.pb.c
        src/protodefs/ReservationStatus.pb.c
        src/protodefs/ReservationsTable.pb.c
        src/protodefs/NumberOfPages.pb.c
        src/protodefs/DeliveryTable.pb.c
    INCLUDE_DIRS
        include/
    REQUIRES 
)
