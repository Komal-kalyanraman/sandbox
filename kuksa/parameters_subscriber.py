from kuksa_client.grpc import VSSClient

with VSSClient('127.0.0.1', 55555) as client:

    for updates in client.subscribe_current_values([
        'Vehicle.AverageSpeed',
        'Vehicle.ADAS.DMS.IsWarning',
        'Vehicle.Cabin.Door.Row1.DriverSide.IsLocked',
    ]):
        # Check if 'Vehicle.AverageSpeed' is in updates before accessing it
        if 'Vehicle.AverageSpeed' in updates:
            avg_speed = updates['Vehicle.AverageSpeed'].value
            print(f"Average Speed km/h : {avg_speed}")
        
        # Check if 'Vehicle.ADAS.DMS.IsWarning' is in updates before accessing it
        if 'Vehicle.ADAS.DMS.IsWarning' in updates:
            DMS_IsWarning = updates['Vehicle.ADAS.DMS.IsWarning'].value
            print(f"DMS warning : {DMS_IsWarning}")
        
        # Check if 'Vehicle.Cabin.Door.Row1.DriverSide.IsLocked' is in updates before accessing it
        if 'Vehicle.Cabin.Door.Row1.DriverSide.IsLocked' in updates:
            Driver_Door_IsLocked = updates['Vehicle.Cabin.Door.Row1.DriverSide.IsLocked'].value
            print(f"Driver door locked : {Driver_Door_IsLocked}")