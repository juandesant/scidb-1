/*
**
* BEGIN_COPYRIGHT
*
* This file is part of SciDB.
* Copyright (C) 2008-2014 SciDB, Inc.
*
* SciDB is free software: you can redistribute it and/or modify
* it under the terms of the AFFERO GNU General Public License as published by
* the Free Software Foundation.
*
* SciDB is distributed "AS-IS" AND WITHOUT ANY WARRANTY OF ANY KIND,
* INCLUDING ANY IMPLIED WARRANTY OF MERCHANTABILITY,
* NON-INFRINGEMENT, OR FITNESS FOR A PARTICULAR PURPOSE. See
* the AFFERO GNU General Public License for the complete license terms.
*
* You should have received a copy of the AFFERO GNU General Public License
* along with SciDB.  If not, see <http://www.gnu.org/licenses/agpl-3.0.html>
*
* END_COPYRIGHT
*/
#ifndef MPI_RANK_SLAVE__H
#define MPI_RANK_SLAVE__H

//
// this file contains routines that are rpc-like "wrappers" for ScaLAPACK calls,
// which allow them to run in a separate process from SciDB.  This is because ScaLAPACK
// runs on MPI, which is not tolerant of node failures.  On node failure, it will either
// kill all MPI processes in the "communcatior" group, or, if it is set not to do that,
// the "communicator" group becomes unuseable until the process is restarted.  Neither of
// these MPI behaviors is compatible with a database server that needs to run 24/7.
//
// Note that the ScaLAPACK routines are actually written in FORTRAN and do not have a
// specific C or C++ API, therfore the types of the arguents in the prototypes are designed
// to permit calling FORTRAN from C++, and it is those types that are required in the
// corresponding wrappers.
//
// This is the "slave-side" wrappers.  For the master-side wrappers, see pdgesvdMaster.hpp
// 
// ScaLAPACK    (SLAVE-SIDE) WRAPPER
// rank         rankSlave
//

// std C++
#include <ostream>

// SciDB
#include <scalapackUtil/scalapackFromCpp.hpp> // home-brew c++ interface to FORTRAN ScaLAPACK
#include <dlaScaLA/slaving/scalapackSlave.hpp> // common argument support

namespace scidb {

//
// MPIRankArgs contains the marshaled arguments for a
// pdgesvdMaster to pdgesvdSlave remote execution
// + two flags, U & V,
// + the size of the input sub-array
// + the global arrays A, U, VT
// + the locallly-replicated array S
//
class MPIRankArgs {
public:
    friend std::ostream& operator<<(std::ostream& os, const MPIRankArgs& a);
    slpp::int_t    NPROW;  // all ops will need these two to set up scalapack via blacs_init
    slpp::int_t    NPCOL;
    slpp::int_t    MYPROW; // all ops will need these to check the that the fake blacs_get_info
    slpp::int_t    MYPCOL; // and the real one returned identical values
    slpp::int_t    MYPNUM; // and the same for blacs_pnum()

    ScalapackArrayArgs IN;
    ScalapackArrayArgs OUT;
};

inline std::ostream& operator<<(std::ostream& os, const MPIRankArgs& a)
{
    os << "IN{" << a.IN << "}"                        << std::endl;
    os << "OUT{" << a.OUT << "}"                      << std::endl;
    return os;
}

slpp::int_t mpirankSlave(void* bufs[], size_t sizes[], unsigned count);

} // namespace scidb

#endif // MPI_RANK_SLAVE__H

