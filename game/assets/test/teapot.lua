Entity = {
    name = "Teapot",
    Components = {
        TransformComponent = {
            m_Position = {x = 0, y = 0, z = 0},
            m_Rotation = {yaw = 0, pitch = 0, roll = 0},
            m_Scale = {x = 1, y = 1, z = 1}
        },
        DiffuseVisualComponent = {
            resFile = "game/assets/test/hello_teapot.obj",
            texturePath = "game/assets/test/yellow.png"
        },
        SphereColliderComponent = {
            m_Radius = 1,
            matName = "Air"
        }
    }
}
