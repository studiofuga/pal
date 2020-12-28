/*
 *   libpal - Automated Placement of Labels Library     http://pal.heig-vd.ch
 *
 *
 *   Copyright (C) 2007, 2008 MIS-TIC, HEIG-VD (University of Applied Sciences Western Switzerland)
 *   Copyright (C) 2008, 2009 IICT-SYSIN, HEIG-VD (University of Applied Sciences Western Switzerland)
 *
 *
 * This file is part of libpal.
 *
 * libpal is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libpal is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with libpal. If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef _PALSTAT_H_
#define _PALSTAT_H_


namespace pal {

    /**
     * Summury of problem
     */
    class PalStat {

        friend class Pal;
        friend class Problem;

    private:
        int nbObjects;
        int nbLabelledObjects;

        int nbLayers;

        char **layersName; //[nbLayers]
        int *layersNbObjects; // [nbLayers]
        int *layersNbLabelledObjects; // [nbLayers]

        PalStat();

    public:

        /**
         * \brief delete stats
         */
        ~PalStat();


        /**
         * \brief the number of object in problem
         */
        int getNbObjects();

        /**
         * \brief the number of objects which are labelled
         */
        int getNbLabelledObjects();

        /**
         *  \brief how many layersare labelled ?
         */
        int getNbLayers();

        /**
         * \brief get a name of the labelled layer 'layerId'
         */
        const char * getLayerName (int layerId);

        /**
         * \brief get the number of object in layer 'layerId'
         */
        int getLayerNbObjects (int layerId);

        /**
         * \brief get the number of object in layer 'layerId' which are labelled
         */
        int getLayerNbLabelledObjects (int layerId);
    };

} // end namespace pal

#endif
