/**
 * Copyright 2019 AbbeyCatUK
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 
#ifndef VIDEO_H
#define VIDEO_H


    void video_print               ( char *pstring );
    void video_printline           ( char *pstring );
    void video_setmode             ( int requested_mode );
    void video_clearscreen         ( void );
    void video_rectangle           ( int x1 , int y1 , int x2 , int y2 );
    void video_point               ( int x , int y );
    void video_line                ( int x1 , int y1 , int x2 , int y2 );
    void video_setcolour		   ( int foreground, int r, int g, int b );


#endif /*VIDEO_H*/
