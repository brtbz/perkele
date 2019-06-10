
void Camera::SetCamera(vec2 _min, vec2 _max)
{
	min = _min;
	max = _max;
	size.x = max.x - min.x;
	size.y = max.y - min.y;
	origo.x = min.x + size.x / 2.0f;
	origo.y = min.y + size.y / 2.0f;
}
void Camera::MoveToNewMin(vec2 new_min)
{
	vec2 change = { new_min.x - min.x, new_min.y - min.y };

	min = new_min;
	max = { max.x + change.x, max.y + change.y };
	origo.x = min.x + size.x / 2.0f;
	origo.y = min.y + size.y / 2.0f;

	vec2 origo_should_be = ConstrainPositionOrigo();
	MoveToNewOrigo(origo_should_be);
}

void Camera::IncreaseSizeBy(float multiplier, vec2 mouse_pos) // zoom out
{
	size.x = size.x * multiplier;
	size.y = size.y * multiplier;

	ConstrainSize();

	min.x = origo.x - size.x / 2.0f;
	min.y = origo.y - size.y / 2.0f;

	max.x = min.x + size.x;
	max.y = min.y + size.y;

	if ( multiplier < 1.0f && mouse_pos.x >= 0)
	{
		//MoveToNewOrigo(mouse_pos);
	}
}

void Camera::SetNewSize(vec2 new_size)
{
	size.x = new_size.x;
	size.y = new_size.y;

	ConstrainSize();

	max.x = origo.x + size.x / 2.0f;
	max.y = origo.y + size.y / 2.0f;

	min.x = origo.x - size.x / 2.0f;
	min.y = origo.y - size.y / 2.0f;
}

void Camera::ConstrainSize()
{
	float max_size_x = viewport_size.x * 3.9f;
	float min_size_x = viewport_size.x * 0.1f;
	float max_size_y = viewport_size.y * 3.9f;
	float min_size_y = viewport_size.y * 0.1f;
	size.x = ClampToF( min_size_x, max_size_x, size.x );
	size.y = ClampToF( min_size_y, max_size_y, size.y );
}

vec2 Camera::ConstrainPositionOrigo()
{
	vec2 desired_origo;

	float min_origo_x = 0.0f;
	float min_origo_y = 0.0f;

	float tile_stride_x = 24.0f;
	float tile_stride_y = 28.0f;

	float max_origo_x = map_width * tile_stride_x;
	float max_origo_y = 20.0f + map_height * tile_stride_y;

	desired_origo.x = ClampToF( min_origo_x, max_origo_x, origo.x );
	desired_origo.y = ClampToF( min_origo_y, max_origo_y, origo.y );

	return desired_origo;
}

vec2 Camera::ConstrainThisOrigo(vec2 origo_seeking_approval)
{
	vec2 desired_origo;

	float min_origo_x = 0.0f;
	float min_origo_y = 0.0f;

	float tile_stride_x = 24.0f;
	float tile_stride_y = 28.0f;

	float max_origo_x = map_width * tile_stride_x;
	float max_origo_y = 20.0f + map_height * tile_stride_y;

	desired_origo.x = ClampToF( min_origo_x, max_origo_x, origo_seeking_approval.x );
	desired_origo.y = ClampToF( min_origo_y, max_origo_y, origo_seeking_approval.y );

	return desired_origo;	
}

void Camera::MoveToNewOrigo(vec2 new_origo)
{
	vec2 pending_new_origo = ConstrainThisOrigo(new_origo);
	vec2 change = { pending_new_origo.x - origo.x, pending_new_origo.y - origo.y };
	origo = pending_new_origo;

	min = { min.x + change.x, min.y + change.y };
	max = { max.x + change.x, max.y + change.y };
}

void InitNewCamera()
{
	vec2 mini = { 0.0f, 0.0f };
	vec2 maxi = viewport_size;
	camera.SetCamera( mini, maxi );
}
