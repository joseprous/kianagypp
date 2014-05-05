/*
kianagy++
Copyright (C) 2014 José Prous

This file is part of kianagy++.

kianagy++ is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

kianagy++ is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with kianagy++.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef ACCUMULATORSMANAGER_HPP
#define ACCUMULATORSMANAGER_HPP

#include <string>
#include <memory>
#include <map>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/median.hpp>
#include <boost/accumulators/statistics/count.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/min.hpp>

typedef boost::accumulators::accumulator_set<
    double,
    boost::accumulators::stats<
        boost::accumulators::tag::mean,
        boost::accumulators::tag::median,
        boost::accumulators::tag::count,
        boost::accumulators::tag::max,
        boost::accumulators::tag::min> > Acc;

typedef std::shared_ptr<Acc> AccSP;

class AccumulatorsManager
{
public:
    AccSP add(std::string name);
    AccSP get(std::string name);
    void print_statistics();
private:
    std::map<std::string,AccSP> accumulators;
};

typedef std::shared_ptr<AccumulatorsManager> AccumulatorsManagerSP;

#endif 
