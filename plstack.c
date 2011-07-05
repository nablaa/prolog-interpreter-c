#include "plstack.h"

PLStackFrame* PLStackFrameCreate(const PLTerm *resolvent,
                                 const PLTerm *goal,
                                 PLTerm *position)
{
	PLStackFrame *frame = (PLStackFrame *)malloc(sizeof(PLStackFrame));
	frame->resolvent = PLTermsCopy(resolvent);
	frame->goal = PLTermsCopy(goal);
	frame->position = position;
	frame->next = NULL;

	return frame;
}

void PLStackFrameFree(PLStackFrame *frame)
{
	if (!frame) {
		return;
	}
	PLTermFree(frame->resolvent);
	PLTermFree(frame->goal);
	free(frame);
}

void PLStackFree(PLStackFrame *frame)
{
	if (!frame) {
		return;
	}
	PLStackFree(frame->next);
	PLStackFrameFree(frame);
}

