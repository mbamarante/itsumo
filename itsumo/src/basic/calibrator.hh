/*
    Copyright (C) 2005 by Ana Lucia Bazzan (bazzan@inf.ufrgs.br)
                          Bruno Castro da Silva (bcs@inf.ufrgs.br)
                          Diogo Becker de Brum (dbrum@inf.ufrgs.br)
            http://www.inf.ufrgs.br/~mas/traffic/siscot

    This file is part of Itsumo.

    Itsumo is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Itsumo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Itsumo; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/
#ifndef __HAS_CALIBRATOR_H
#define __HAS_CALIBRATOR_H

#include "types.hh"

#include "simulation.hh"
#include "topologyelement.hh"
#include "car.hh"
#include "driver-loader.hh"
#include "../drivers/nasch.hh"

#define SOURCE 222
#define SINK 666


/***********************************************************
                        CALIBRATOR
************************************************************/

class Calibrator : public TopologyElement {
private:

  Node *parentNode;
  iteration_t timeBegin;
  
  // this field will be interpreted as the laneset where the calibrator
  // should put cars (in the case of a Source), or the laneset from where
  // the calibrator is going to remove cars (in the case of a Sink)
  LaneSet *associatedLaneSet;

  // either SOURCE or SINK
  int type;
  
  double activationProbability;

protected:


public:

  int running;

  void setParentNode(Node *_parentNode);
  Node *getParentNode();

  void setAssociatedLaneSet(LaneSet *_assocLaneSet);
  LaneSet *getAssociatedLaneSet();

  void setTimeBegin(iteration_t _timeBegin);
  iteration_t getTimeBegin();

  int getType();
  void setType(int _type);

  double getActivationProbability();
  void setActivationProbability(double _probability);

  virtual void update(iteration_t currentIteration) =0;

  Calibrator (iteration_t _timeBegin, Node *parentNode, LaneSet *_assocLaneSet);

  virtual ~Calibrator ();
};







/***********************************************************
                           SOURCE
************************************************************/



class Source : public Calibrator {
private:
  iteration_t timeStep;

  iteration_t source_iter;
  int current_behavior;

  int sourceType;
  int constantFlow;

public:



  iteration_t getTimeStep();
  void setTimeStep(iteration_t _timeStep);

  void update(iteration_t currentIteration);
  void act();

  // finds the best place to insert a car
  dist_t bestPlaceOnLane(Lane *lane);

  // sets the source type {constant prob., variable prob., constant flow, variable flow}
  void setSourceType(int _sourceType);
  int getSourceType();


  void setVariableProb(variable_probability_vector_t);
  double getVariableProb();

  void setVariableFlow(variable_flow_vector_t);
  int getVariableFlow();

  void setConstantFlow(int _constantFlow);
  int getConstantFlow();

  variable_flow_vector_t variableFlowTable;
  variable_probability_vector_t variableProbabilityTable;

  Source (int _sourceType,
	  iteration_t _timeBegin, 
	  Node *parentNode, 
	  iteration_t _timeStep, 
	  LaneSet *_assocLaneSet);

  ~Source ();
};






/***********************************************************
                           SINK
************************************************************/


class Sink : public Calibrator {
private:


public:

  void update(iteration_t currentIteration);

  int act(Car *car);

  Sink (iteration_t _timeBegin, 
	Node *parentNode,
	LaneSet *_assocLaneSet);

  ~Sink ();
};







#endif
