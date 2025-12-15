#ifndef TRAJECTORY_H
#define TRAJECTORY_H

typedef struct {
    float x;
    float y;
    float theta;
} Pose2D;

void traj_init(void);
void traj_update(float v_l, float v_r);

Pose2D traj_get_pose(void);

#endif
