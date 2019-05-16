
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
}

void Camera::IncreaseSizeBy(float multiplier) // zoom out
{
	size.x = size.x * multiplier;
	size.y = size.y * multiplier;

	min.x = origo.x - size.x / 2.0f;
	min.y = origo.y - size.y / 2.0f;

	max.x = min.x + size.x;
	max.y = min.y + size.y;
}

void Camera::SetNewSize(vec2 new_size)
{
	size.x = new_size.x;
	size.y = new_size.y;

	max.x = origo.x + size.x / 2.0f;
	max.y = origo.y + size.y / 2.0f;

	min.x = origo.x - size.x / 2.0f;
	min.y = origo.y - size.y / 2.0f;
}

void Camera::MoveToNewOrigo(vec2 new_origo)
{
	vec2 change = { new_origo.x - origo.x, new_origo.y - origo.y };

	origo = new_origo;
	min = { min.x + change.x, min.y + change.y };
	max = { max.x + change.x, max.y + change.y };
}

void InitNewCamera()
{
	vec2 mini = { 0.0f, 0.0f };
	//vec2 maxi = { 1920.0f, 1080.0f};
	vec2 maxi = viewport_size;
	camera.SetCamera( mini, maxi );
}
