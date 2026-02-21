#include "Ball.h"
#include <cmath>

Ball::Ball(b2World& world)
{
    SCALE = 30.f;   // 30 pixels = 1 meter
    radius = 35.f;
    color = sf::Color::Black;

    // Animations
    spikeProgress = 0.0f;
    spikeSpeed = 0.7f;   // animation for spikes

    // ---------------------------
    // Create Physics Body
    // ---------------------------

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;               // Dynamic = affected by gravity
    bodyDef.position.Set(400.f / SCALE, 100.f / SCALE);

    body = world.CreateBody(&bodyDef);

    // Create Visual Shape
    b2CircleShape circleShape;
    circleShape.m_radius = radius / SCALE;
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setFillColor(color);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixture = body->CreateFixture(&fixtureDef);
    spikeMode = false; // initially start in bouncy mode
    bouncyMode(fixture); 
}

void Ball::bouncyMode(b2Fixture* fix)
{
    fix->SetDensity(0.4f); // heaviniess (mass)
	fix->SetFriction(0.2f); // grip on surfaces
	fix->SetRestitution(0.4f); // bounciness

	body->SetLinearDamping(0.12f); // air resistance
    body->SetAngularDamping(0.1f); // rotational resistance to prevent excessive spinning
	body->SetGravityScale(0.65f); // reduce gravity for a floatier feel
    body->ResetMassData();
}

void Ball::spikeyMode(b2Fixture * fix)
{
	fix->SetDensity(2.0f); // heaviniess (mass)
	fix->SetFriction(1.5f); // grip on surfaces (high friction to prevent sliding when spiked)
	fix->SetRestitution(0.03f); // low bounciness to feel more "stuck" when spiked

    body->SetLinearDamping(0); // air resistance
    body->SetAngularDamping(0.5f); // rotational resistance to prevent excessive spinning
    body->SetGravityScale(1.5f); // increase gravity for a heavier feel
    body->ResetMassData();
}

void Ball::update()
{
    // Get position from physics world
    b2Vec2 position = body->GetPosition();
    float angle = body->GetAngle();

    // Convert meters → pixels
    shape.setPosition(position.x * SCALE, position.y * SCALE);

    // Convert radians → degrees
    shape.setRotation(angle * 180.f / b2_pi);

	// Update spike animation progress.
    float deltaTime = 1.f / 60.f;  // since you use fixed timestep
    if (spikeMode)
    {
        spikeProgress += spikeSpeed * deltaTime;
        if (spikeProgress > 1.f)
            spikeProgress = 1.f;
    }
    else
    {
        spikeProgress -= spikeSpeed * deltaTime;
        if (spikeProgress < 0.f)
            spikeProgress = 0.f;
    }
}

void Ball::draw(sf::RenderWindow& window)
{
    window.draw(shape);

    // Draw spikes while they are visible (growing or retracting).
    // This ensures the retract animation is rendered even after spikeMode is toggled off.
    if (spikeProgress > 0.001f)
    {
        drawSpikes(window);
    }
}

void Ball::jump()
{
    b2Vec2 velocity = body->GetLinearVelocity();

    // Allow jump when roughly on/near the ground (not too high vertical speed)
    // Using a larger threshold is simpler than contact detection for this demo.
    if (std::abs(velocity.y) < 0.2f)
    {
        float impulseStrength = -4.8f;  // negative = upward (tuned for feel)

        body->ApplyLinearImpulse(
            b2Vec2(0.0f, impulseStrength),
            body->GetWorldCenter(),
            true
        );
    }
}

void Ball::move(float direction)
{
    // Make horizontal movement responsive by applying an impulse toward a
    // target horizontal speed instead of a slow continuous force.
    const float maxSpeed = 0.8f; // meters per second (box2d units)

    b2Vec2 vel = body->GetLinearVelocity();
    float desiredVelX = direction * maxSpeed;
    float velChange = desiredVelX - vel.x;

    // Calculate impulse = mass * deltaV
    float mass = body->GetMass();
    float impulseX = mass * velChange;

    // Apply impulse for an immediate change in horizontal velocity
    body->ApplyLinearImpulse(b2Vec2(impulseX, 0.0f), body->GetWorldCenter(), true);

    // Clamp absolute horizontal speed to maxSpeed to avoid runaway velocities
    vel = body->GetLinearVelocity();
    if (std::abs(vel.x) > maxSpeed)
    {
        vel.x = (vel.x > 0.0f) ? maxSpeed : -maxSpeed;
        body->SetLinearVelocity(vel);
    }
}

void Ball::toggleSpikeMode()
{
    spikeMode = !spikeMode;

    if (spikeMode)
        spikeyMode(fixture);
    else
        bouncyMode(fixture);
}

void Ball::drawSpikes(sf::RenderWindow& window)
{
    int spikeCount = 6;

    // spike geometry relative to ball center (0,0). We position the convex
    // shape at the ball center, then rotate it so the tip points outward.
    float maxSpikeLength = radius * 0.9f;
    float spikeLength = maxSpikeLength * spikeProgress;
    // base sits on circle perimeter
    float baseRadius = radius - 1; // -1 to prevent gap
    // ensure tip is outside the circle (extra pixel to avoid overlap when spikeLength is small)
    float tipRadius = radius + spikeLength + 1.0f;
    float baseHalf = spikeLength * 0.275f; // half-width of the spike base

    for (int i = 0; i < spikeCount; i++)
    {
        float angleDeg = (360.f / spikeCount) * i;

        sf::ConvexShape spike;
        spike.setPointCount(3);

        // Points defined relative to the ball center (origin at ball center).
        // Tip: straight up along negative Y, base left and right on circle rim.
        spike.setPoint(0, sf::Vector2f(0.f, -tipRadius)); // tip (outside circle)
        spike.setPoint(1, sf::Vector2f(-baseHalf, -baseRadius));           // base left
        spike.setPoint(2, sf::Vector2f(baseHalf, -baseRadius));            // base right

        spike.setFillColor(color);

        // Place triangle origin at ball center so rotation pivots around center.
        spike.setPosition(shape.getPosition());
        spike.setRotation(shape.getRotation() + angleDeg);

        window.draw(spike);
    }
}